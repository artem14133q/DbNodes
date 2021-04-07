#include "QLabel"

#include "NewProject.h"

#include "DefaultProjectSettingsDictionary.h"

#include "../helper.h"

namespace DbNodes::Modals {

    NewProject::NewProject(QWidget *parent): Abstract::AbstractSettingModal(parent)
    {
        setWindowTitle("Create new project");
        setFixedWidth(500);

        prepareWidget();

        show();
    }

    void NewProject::exit()
    {
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(false);

        Abstract::AbstractSettingModal::exit();
    }

    void NewProject::validPropertyName(QLineEdit *widget)
    {
        auto state = widget->text() == "";

        widget->setProperty("warning", state);
        widget->setPlaceholderText("Must be field");

        style()->unpolish(widget);
        style()->polish(widget);

        enableConfirm(!state);
    }

    void NewProject::confirm()
    {
        emit createProjectSignal(newSettingsMap);

        AbstractSettingModal::confirm();
    }

    void NewProject::initSettingsUi()
    {
        nameEdit = createTextSetting("Project name:", NAME_KEY,
            [this] (QLineEdit *lineEdit) {
                validPropertyName(lineEdit);
            }
        );

        auto widthSpin = createSpinboxSetting("Workspace width:", WIDTH_KEY, {2000, 8000});
        widthSpin->setPrefix("px ");

        auto heightSpin = createSpinboxSetting("Workspace height:", HEIGHT_KEY, {2000, 8000});
        heightSpin->setPrefix("px ");

        setSpacing(15);

        // Create buttons =>

        createButton("Cancel", this, [this] {
            exit();
        });

        createProjectButton = createButton("Create", this, [this] {
            confirm();
        });

        createProjectButton->setDisabled(true);
    }

    QVariant NewProject::getDefaultSetting(const QString &name)
    {
        return Dictionaries::DefaultProjectSettingsDictionary::getValue(name);
    }

    void NewProject::enableConfirm(const bool &enable)
    {
        createProjectButton->setEnabled(enable);
        AbstractSettingModal::enableConfirm(enable);
    }

    void NewProject::afterInitUi()
    {
        nameEdit->setFocus();
        nameEdit->selectAll();
    }
}