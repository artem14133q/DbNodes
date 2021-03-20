//
// Created by artem on 20.03.2021.
//

#include "QStyle"
#include "QApplication"

#include "Repository.h"
#include "Selectable.h"
#include "../helper.h"

namespace DbNodes::Utils::MultipleSelection {

    Repository::Repository(QWidget *parent): QObject(parent) {}

    void Repository::unSelectNodes()
    {
        foreach (const NODE_POINTER &node, selectedNodes) {
            setSelectToNode(node, false);
        }

        selectedNodes.clear();
    }

    void Repository::setSelectToNode(const NODE_POINTER &node, const bool &select)
    {
        auto unConstNode = const_cast<NODE_POINTER &>(node);

        unConstNode->setProperty("selected", select);
        unConstNode->style()->unpolish(unConstNode);
        unConstNode->style()->polish(unConstNode);
    }

    void Repository::insertNodeToSelectionList(const NODE_POINTER &node)
    {
        if (!selectedNodes.contains(node)) {
            selectedNodes.push_back(node);
        }
    }

    void Repository::removeNodeFromSelectionList(const NODE_POINTER &node)
    {
        selectedNodes.removeAll(node);
    }

    void Repository::moveSelectedNode(QObject *node, const QPoint &delta)
    {
        Helper::removeDeletedItems<Widgets::Node>(selectedNodes);

        foreach (const NODE_POINTER &currentNode, selectedNodes) {
            if (currentNode == QPointer(dynamic_cast<Widgets::Node *>(node))) {
                continue;
            }

            currentNode->blockSignals(true);
            currentNode->move(currentNode->pos() + delta);
            currentNode->blockSignals(false);
        }
    }

    void Repository::start(const QPoint &mousePos)
    {
        mousePressed = true;
        mouseStartPos = mousePos;

        bool ctrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
        bool altPressed = QApplication::keyboardModifiers() & Qt::AltModifier;

        if (!(ctrlPressed || altPressed)) {
            unSelectNodes();
        }
    }

    void Repository::move(const QPoint &mousePos, const QList<NODE_POINTER> &nodes)
    {
        if (mousePressed) {
            mouseCurrentPos = mousePos;

            QRect selectionRect(mouseStartPos, mouseCurrentPos);

            bool ctrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
            bool altPressed = QApplication::keyboardModifiers() & Qt::AltModifier;

            if (!(ctrlPressed || altPressed)) {
                unSelectNodes();
            }

            foreach (const NODE_POINTER &node, nodes) {
                if (selectionRect.intersects(node->geometry())) {
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

    void Repository::initDefaultsConnections(const NODE_POINTER &node)
    {
        Selectable *selectableNode = node->getSelectable();

        connect(selectableNode, &Selectable::unSelectNodesSignal, this, &Repository::unSelectNodes);
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
}
