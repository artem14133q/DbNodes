//
// Created by artem on 20.03.2021.
//

#include "QStyle"
#include "QApplication"
#include "QDebug"

#include "Repository.h"
#include "Selectable.h"
#include "../helper.h"

namespace DbNodes::Utils::MultipleSelection {

    Repository::Repository(QWidget *parent): QObject(parent) {}

    void Repository::unselectNodes()
    {
        foreach (const Abstract::NodePtr &table, selectedNodes) {
            setSelectToNode(table, false);
        }

        selectedNodes.clear();
    }

    void Repository::setSelectToNode(const Abstract::NodePtr &node, const bool &select)
    {
        auto unConstNode = const_cast<Abstract::NodePtr &>(node);

        unConstNode->setProperty("selected", select);
        unConstNode->style()->unpolish(unConstNode);
        unConstNode->style()->polish(unConstNode);
    }

    void Repository::insertNodeToSelectionList(const Abstract::NodePtr &node)
    {
        if (!selectedNodes.contains(node)) {
            selectedNodes.push_back(node);
        }
    }

    void Repository::removeNodeFromSelectionList(const Abstract::NodePtr &node)
    {
        selectedNodes.removeAll(node);
    }

    void Repository::moveSelectedNode(QObject *node, const QPoint &delta)
    {
        Helper::removeDeletedItems<Abstract::AbstractNode>(selectedNodes);

        foreach (const Abstract::NodePtr &currentNode, selectedNodes) {
            if (currentNode == QPointer(dynamic_cast<Abstract::AbstractNode *>(node))) {
                continue;
            }

            currentNode->blockSignals(true);
            currentNode->restrictedMove(currentNode->pos() + delta);
            currentNode->blockSignals(false);
        }
    }

    void Repository::start(const QPoint &mousePos)
    {
        mousePressed = true;

        mouseStartPos = mousePos;
        mouseCurrentPos = mousePos;

        bool ctrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
        bool altPressed = QApplication::keyboardModifiers() & Qt::AltModifier;

        if (!(ctrlPressed || altPressed)) {
            unselectNodes();
        }
    }

    void Repository::move(const QPoint &mousePos, const QList<Abstract::NodePtr> &nodes)
    {
        if (mousePressed) {
            mouseCurrentPos = mousePos;

            QRect selectionRect(mouseStartPos, mouseCurrentPos);

            bool ctrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
            bool altPressed = QApplication::keyboardModifiers() & Qt::AltModifier;

            if (!(ctrlPressed || altPressed)) {
                unselectNodes();
            }

            foreach (const Abstract::NodePtr &node, nodes) {
                if (selectionRect.intersects(node->geometry()) && node->getSelectionUtil()->enabled()) {
                    if (altPressed) {
                        setSelectToNode(node, false);

                        removeNodeFromSelectionList(node);
                    } else {
                        setSelectToNode(node, true);

                        insertNodeToSelectionList(node);
                    }
                }
            }

            parentWidget()->update();
        }
    }

    QWidget *Repository::parentWidget()
    {
        return dynamic_cast<QWidget *>(parent());
    }

    void Repository::stop()
    {
        mousePressed = false;
        parentWidget()->update();
    }

    void Repository::drawSelectionRect(QPainter &painter)
    {
        if (mousePressed) {
            painter.setPen(QPen(QColor(100, 100, 100), 2, Qt::DashLine, Qt::FlatCap));
            painter.drawRect(QRect(mouseStartPos, mouseCurrentPos));
        }
    }

    void Repository::initDefaultsConnections(const Abstract::NodePtr &node)
    {
        Selectable *selectableNode = node->getSelectionUtil();

        connect(selectableNode, &Selectable::unSelectNodesSignal, this, &Repository::unselectNodes);
        connect(selectableNode, &Selectable::moveSignal, this, &Repository::moveSelectedNode);

        connect(selectableNode, &Selectable::selectCurrentNodeSignal, this, [this, node] {
            setSelectToNode(node, true);
            insertNodeToSelectionList(node);
        });

        connect(selectableNode, &Selectable::unSelectCurrentNodeSignal, this, [this, node] {
            setSelectToNode(node, false);
            removeNodeFromSelectionList(node);
        });
    }

    Repository::~Repository()
    {
        selectedNodes.clear();

        QObject::deleteLater();
    }

    void Repository::deleteSelected()
    {
        if(selectedNodes.isEmpty()) return;

        QList<Abstract::NodePtr> others = Helper::filter<Abstract::NodePtr>(
            selectedNodes,
            [] (const Abstract::NodePtr &node) -> bool {
                if (node->objectName() == "TableNode") {
                    node->deleteLater();
                    return false;
                } else {
                    return true;
                }
            }
        );

        foreach (const Abstract::NodePtr &node, others) {
            // Call override destructor. When PathPoint signal that it delete.
            // When TableNode remove PathPoint from list.
            delete node;
        }

        selectedNodes.clear();
    }

    void Repository::initDefaultActionsForUtil(QMenu *menu)
    {
        auto deleteSelectedAction = menu->addAction("Delete selected");
        if (selectedNodes.isEmpty()) deleteSelectedAction->setDisabled(true);

        connect(deleteSelectedAction, &QAction::triggered, this, &Repository::deleteSelected);
    }
}
