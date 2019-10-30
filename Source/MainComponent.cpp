/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

TableTutorialComponent::TableTutorialComponent()
{
    loadData();
    
    addAndMakeVisible(table);
    table.setColour(ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness(1);
    
    if (columnList != nullptr)
    {
        forEachXmlChildElement(*columnList, columnXml)
        {
            table.getHeader().addColumn(columnXml->getStringAttribute("name"),
                                        columnXml->getIntAttribute("columnId"),
                                        columnXml->getIntAttribute("width"),
                                        50,
                                        400,
                                        TableHeaderComponent::defaultFlags);
        }
    }
    table.getHeader().setSortColumnId(1, true);
    table.setMultipleSelectionEnabled(true);
}
int TableTutorialComponent::getNumRows()
{
    return numRows;
}
void TableTutorialComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId).interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll(Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(alternateColour);
}
void TableTutorialComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
    g.setFont(font);
    if (auto* rowElement = dataList->getChildElement(rowNumber))
    {
        auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
        g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}
void TableTutorialComponent::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        TutorialDataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
        dataList->sortChildElements (sorter);
        
        table.updateContent();
    }
}
Component* TableTutorialComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
                                    Component* existingComponentToUpdate)
{
    if (columnId == 9)
    {
        auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);
        
        if (selectionBox == nullptr)
            selectionBox = new SelectionColumnCustomComponent (*this);
        
        selectionBox->setRowAndColumn (rowNumber, columnId);
        return selectionBox;
    }
    
    if (columnId == 8)
    {
        auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
        
        if (textLabel == nullptr)
            textLabel = new EditableTextCustomComponent (*this);
        
        textLabel->setRowAndColumn (rowNumber, columnId);
        return textLabel;
    }
    
    jassert (existingComponentToUpdate == nullptr);
    return nullptr;
}
int TableTutorialComponent::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 9)
        return 50;
    
    int widest = 32;
    for (auto i = getNumRows(); --i >= 0;)
    {
        if (auto* rowElement = dataList->getChildElement (i))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
            
            widest = jmax (widest, font.getStringWidth (text));
        }
    }
    return widest + 8;
}
int TableTutorialComponent::getSelection (const int rowNumber) const
{
    return dataList->getChildElement (rowNumber)->getIntAttribute ("Select");
}
void TableTutorialComponent::setSelection (const int rowNumber, const int newSelection)
{
    dataList->getChildElement (rowNumber)->setAttribute ("Select", newSelection);
}
String TableTutorialComponent::getText (const int columnNumber, const int rowNumber) const
{
    return dataList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}
void TableTutorialComponent::setText (const int columnNumber, const int rowNumber, const String& newText)
{
    const auto& columnName = table.getHeader().getColumnName (columnNumber);
    dataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
}
void TableTutorialComponent::resized()
{
    table.setBoundsInset (BorderSize<int> (8));
}
TableTutorialComponent::EditableTextCustomComponent::EditableTextCustomComponent (TableTutorialComponent& td)    : owner (td)
{
    setEditable (false, true, false);
}
void TableTutorialComponent::EditableTextCustomComponent::mouseDown (const MouseEvent& event)
{
    owner.table.selectRowsBasedOnModifierKeys (row, event.mods, false);
    
    Label::mouseDown (event);
}
void TableTutorialComponent::EditableTextCustomComponent::textWasEdited()
{
    owner.setText (columnId, row, getText());
}
void TableTutorialComponent::EditableTextCustomComponent::setRowAndColumn (const int newRow, const int newColumn)
{
    row = newRow;
    columnId = newColumn;
    setText (owner.getText(columnId, row), dontSendNotification);
}
TableTutorialComponent::SelectionColumnCustomComponent::SelectionColumnCustomComponent (TableTutorialComponent& td) : owner (td)
{
    addAndMakeVisible (toggleButton);
    
    toggleButton.onClick = [this] { owner.setSelection (row, (int) toggleButton.getToggleState()); };
}

void TableTutorialComponent::SelectionColumnCustomComponent::resized()
{
    toggleButton.setBoundsInset (BorderSize<int> (2));
}

void TableTutorialComponent::SelectionColumnCustomComponent::setRowAndColumn (int newRow, int newColumn)
{
    row = newRow;
    columnId = newColumn;
    toggleButton.setToggleState ((bool) owner.getSelection (row), dontSendNotification);
}
TableTutorialComponent::TutorialDataSorter::TutorialDataSorter (const String& attributeToSortBy, bool forwards)
: attributeToSort (attributeToSortBy), direction (forwards ? 1 : -1)
{
}
int TableTutorialComponent::TutorialDataSorter::compareElements (XmlElement* first, XmlElement* second) const
{
    auto result = first->getStringAttribute (attributeToSort)
    .compareNatural (second->getStringAttribute (attributeToSort));
    
    if (result == 0)
        result = first->getStringAttribute ("ID")
        .compareNatural (second->getStringAttribute ("ID"));
    
    return direction * result;
}
void TableTutorialComponent::loadData()
{
    auto dir = File::getCurrentWorkingDirectory();
    int numTries = 0;
    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
        dir = dir.getParentDirectory();
    
    auto tableFile = dir.getChildFile("Resources").getChildFile("TableData.xml");
    if (tableFile.exists())
    {
        tutorialData = XmlDocument::parse(tableFile);
        dataList = tutorialData->getChildByName("DATA");
        columnList = tutorialData->getChildByName("HEADERS");
        numRows = dataList->getNumChildElements();
    }
}
String TableTutorialComponent::getAttributeNameForColumnId (const int columnId) const
{
    forEachXmlChildElement(*columnList, columnXml)
    {
        if(columnXml->getIntAttribute("columnId") == columnId)
            return columnXml->getStringAttribute("name");
    }
    return {};
}

MainComponent::MainComponent()
{
    addAndMakeVisible(table);
    setSize (1200, 600);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    table.setBounds(getLocalBounds());
}
