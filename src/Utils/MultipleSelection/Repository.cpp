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

    void Repository::unSelectTables()
    {
        foreach (const TABLE_POINTER &table, selectedTables) {
            setSelectToTable(table, false);
        }

        selectedTables.clear();
    }

    void Repository::setSelectToTable(const TABLE_POINTER &table, const bool &select)
    {
        auto unConstTable = const_cast<TABLE_POINTER &>(table);

        unConstTable->setProperty("selected", select);
        unConstTable->style()->unpolish(unConstTable);
        unConstTable->style()->polish(unConstTable);
    }

    void Repository::insertTableToSelectionList(const TABLE_POINTER &table)
    {
        if (!selectedTables.contains(table)) {
            selectedTables.push_back(table);
        }
    }

    void Repository::removeTableFromSelectionList(const TABLE_POINTER &table)
    {
        selectedTables.removeAll(table);
    }

    void Repository::moveSelectedTable(QObject *table, const QPoint &delta)
    {
        Helper::removeDeletedItems<Nodes::TableNode>(selectedTables);

        foreach (const TABLE_POINTER &currentTable, selectedTables) {
            if (currentTable == QPointer(dynamic_cast<Nodes::TableNode *>(table))) {
                continue;
            }

            currentTable->blockSignals(true);
            currentTable->move(currentTable->pos() + delta);
            currentTable->blockSignals(false);
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
            unSelectTables();
        }
    }

    void Repository::move(const QPoint &mousePos, const QList<TABLE_POINTER> &tables)
    {
        if (mousePressed) {
            mouseCurrentPos = mousePos;

            QRect selectionRect(mouseStartPos, mouseCurrentPos);

            bool ctrlPressed = QApplication::keyboardModifiers() & Qt::ControlModifier;
            bool altPressed = QApplication::keyboardModifiers() & Qt::AltModifier;

            if (!(ctrlPressed || altPressed)) {
                unSelectTables();
            }

            foreach (const TABLE_POINTER &table, tables) {
                if (selectionRect.intersects(table->geometry())) {
                    if (altPressed) {
                        setSelectToTable(table, false);

                        removeTableFromSelectionList(table);
                    } else {
                        setSelectToTable(table, true);

                        insertTableToSelectionList(table);
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

    void Repository::initDefaultsConnections(const TABLE_POINTER &table)
    {
        Selectable *selectableTable = table->getSelectable();

        connect(selectableTable, &Selectable::unSelectTablesSignal, this, &Repository::unSelectTables);
        connect(selectableTable, &Selectable::moveSignal, this, &Repository::moveSelectedTable);

        connect(selectableTable, &Selectable::selectCurrentTableSignal, this, [this, table] {
            setSelectToTable(table, true);
            insertTableToSelectionList(table);
        });

        connect(selectableTable, &Selectable::unSelectCurrentTableSignal, this, [this, table] {
            setSelectToTable(table, false);
            removeTableFromSelectionList(table);
        });
    }

    Repository::~Repository()
    {
        selectedTables.clear();

        QObject::deleteLater();
    }
}
