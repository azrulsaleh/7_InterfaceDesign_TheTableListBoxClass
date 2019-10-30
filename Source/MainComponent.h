/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TableTutorialComponent    : public Component, public TableListBoxModel
{
public:
    TableTutorialComponent();
    int getNumRows() override;
    void paintRowBackground (Graphics&, int, int, int, bool) override;
    void paintCell(Graphics&, int, int, int, int, bool) override;
    void sortOrderChanged(int, bool) override;
    Component* refreshComponentForCell (int, int, bool, Component*) override;
    int getColumnAutoSizeWidth(int) override;
    int getSelection (const int rowNumber) const;
    void setSelection (const int rowNumber, const int newSelection);
    String getText (const int columnNumber, const int rowNumber) const;
    void setText (const int columnNumber, const int rowNumber, const String& newText);
    void resized() override;
private:
    TableListBox table { {}, this };
    Font font { 14.0f };
    std::unique_ptr<XmlElement> tutorialData;
    XmlElement* columnList = nullptr;
    XmlElement* dataList = nullptr;
    int numRows = 0;
    class EditableTextCustomComponent  : public Label
    {
    public:
        EditableTextCustomComponent (TableTutorialComponent& td);
        void mouseDown (const MouseEvent& event) override;
        void textWasEdited() override;
        void setRowAndColumn (const int newRow, const int newColumn);
    private:
        TableTutorialComponent& owner;
        int row, columnId;
        Colour textColour;
    };
    
    //==============================================================================
    class SelectionColumnCustomComponent    : public Component
    {
    public:
        SelectionColumnCustomComponent (TableTutorialComponent& td);
        void resized() override;
        void setRowAndColumn (int newRow, int newColumn);
    private:
        TableTutorialComponent& owner;
        ToggleButton toggleButton;
        int row, columnId;
    };
    
    //==============================================================================
    class TutorialDataSorter
    {
    public:
        TutorialDataSorter (const String& attributeToSortBy, bool forwards);        
        int compareElements (XmlElement* first, XmlElement* second) const;
        
    private:
        String attributeToSort;
        int direction;
    };
    void loadData();
    String getAttributeNameForColumnId (const int) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TableTutorialComponent)
};

class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    TableTutorialComponent table;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
