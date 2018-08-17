#pragma once

#include "Panel.h"
#include <vector>
#include <functional>
#include "RowDefinitionCollection.h"
#include "ColumnDefinitionCollection.h"
#include "Internals/DefinitionState.h"
#include "../Common/Numeric.h"

using namespace CoreVar::Xaml::UI::Internals;


namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Grid : public Panel
			{
			private:
				RowDefinitionCollection * _rowDefinitions;
				ColumnDefinitionCollection* _columnDefinitions;

				struct ItemMeasurement
				{
					double assigned_size;
					double measured_size;
					int itemMultipliers;
				};

				std::vector<ItemMeasurement*>* _rowMeasurements = NULL;
				std::vector<ItemMeasurement*>* _columnMeasurements = NULL;

			protected:

				void refreshMeasurements()
				{
					if (_rowMeasurements == NULL)
					{
						_rowMeasurements = new std::vector<ItemMeasurement*>();
					}
					if (_columnMeasurements == NULL)
					{
						_columnMeasurements = new std::vector<ItemMeasurement*>();
					}

					int rowCount = _rowDefinitions->count();
					if (rowCount == 0)
					{
						rowCount = 1;
					}

					int difference = rowCount - _rowMeasurements->size();
					if (difference < 0)
					{
						int removeCount = -difference;
						for (int i = 0; i < removeCount; i++)
						{
							delete _rowMeasurements->at(0);
							_rowMeasurements->erase(_rowMeasurements->begin());
						}
					}
					else if (difference > 0)
					{
						int addCount = difference;
						for (int i = 0; i < addCount; i++)
						{
							_rowMeasurements->push_back(new ItemMeasurement());
						}
					}

					int columnCount = _columnDefinitions->count();
					if (columnCount == 0)
					{
						columnCount = 1;
					}

					difference = columnCount - _columnMeasurements->size();
					if (difference < 0)
					{
						int removeCount = -difference;
						for (int i = 0; i < removeCount; i++)
						{
							delete _columnMeasurements->at(0);
							_columnMeasurements->erase(_columnMeasurements->begin());
						}
					}
					else if (difference > 0)
					{
						int addCount = difference;
						for (int i = 0; i < addCount; i++)
						{
							_columnMeasurements->push_back(new ItemMeasurement());
						}
					}
				}

				Size measureOverride(Size availableSize)
				{
					refreshMeasurements();

					XamlType* gridElementType = services->commonTypes->Xaml_UI_Grid();
					XamlAttachedProperty* rowProperty = services->commonTypes->Xaml_UI_Grid_Row();
					XamlAttachedProperty* rowSpanProperty = services->commonTypes->Xaml_UI_Grid_RowSpan();
					XamlAttachedProperty* columnProperty = services->commonTypes->Xaml_UI_Grid_Column();
					XamlAttachedProperty* columnSpanProperty = services->commonTypes->Xaml_UI_Grid_ColumnSpan();
					int rowCount = _rowDefinitions->count();
					if (rowCount == 0)
					{
						rowCount = 1;
					}
					int rowMultipliers = 0;
					double fixedHeight = 0;
					if (_rowDefinitions->count() > 0)
					{
						for (int i = 0; i < rowCount; i++)
						{
							ItemMeasurement* rowMeasurement = _rowMeasurements->at(i);
							rowMeasurement->measured_size = Double_NaN;
							rowMeasurement->assigned_size = Double_NaN;
							rowMeasurement->itemMultipliers = 0;
							RowDefinition* rowDefinition = _rowDefinitions->get_item(i);
							GridLength* rowDefinitionHeight = rowDefinition->get_height_pointer();
							switch (rowDefinitionHeight->gridUnitType)
							{
							case GridUnitType::Star:
								rowMultipliers += rowMeasurement->itemMultipliers = isnan(rowDefinitionHeight->value) ? 1 : (int)rowDefinitionHeight->value;
								break;
							case GridUnitType::Pixel:
								fixedHeight += rowMeasurement->measured_size = rowMeasurement->assigned_size = rowDefinitionHeight->value;
								break;
							case GridUnitType::Auto:
								break;
							}
						}
					}
					else
					{
						ItemMeasurement* rowMeasurement = _rowMeasurements->at(0);
						rowMeasurement->measured_size = Double_NaN;
						rowMeasurement->assigned_size = Double_NaN;
						rowMeasurement->itemMultipliers = rowMultipliers = 1;
					}

					int columnCount = _columnDefinitions->count();
					int columnMultipliers = 0;
					double fixedWidth = 0;

					if (_columnDefinitions->count() > 0)
					{
						for (int i = 0; i < columnCount; i++)
						{
							ItemMeasurement* columnMeasurement = _columnMeasurements->at(i);
							columnMeasurement->measured_size = Double_NaN;
							columnMeasurement->assigned_size = Double_NaN;
							columnMeasurement->itemMultipliers = 0;
							ColumnDefinition* columnDefinition = _columnDefinitions->get_item(i);
							GridLength* columnDefinitionWidth = columnDefinition->get_width_pointer();
							switch (columnDefinitionWidth->gridUnitType)
							{
							case GridUnitType::Star:
								columnMultipliers += columnMeasurement->itemMultipliers = isnan(columnDefinitionWidth->value) ? 1 : (int)columnDefinitionWidth->value;
								break;
							case GridUnitType::Pixel:
								fixedWidth += columnMeasurement->measured_size = columnMeasurement->assigned_size = columnDefinitionWidth->value;
								break;
							case GridUnitType::Auto:
								break;
							}
						}
					}
					else
					{
						ItemMeasurement* columnMeasurement = _columnMeasurements->at(0);
						columnMeasurement->measured_size = Double_NaN;
						columnMeasurement->assigned_size = Double_NaN;
						columnMeasurement->itemMultipliers = columnMultipliers = 1;
					}

					int childCount = get_children()->count();
					for (int i = 0; i < childCount; i++)
					{
						UIElement* uiElement = get_children()->get_item(i);

						void* rowPointer = uiElement->get_value(rowProperty);
						int rowValue = rowPointer == NULL ? 0 : *(int*)rowPointer;
						void* rowSpanPointer = uiElement->get_value(rowSpanProperty);
						int rowSpanValue = rowSpanPointer == NULL ? 1 : *(int*)rowSpanPointer;
						int lastRow = rowValue + rowSpanValue - 1;

						double currentOccupiedHeight = 0;
						for (int x = 0; x < _rowDefinitions->count(); x++)
						{
							if (x >= rowValue || x <= lastRow)
							{
								continue;
							}
							ItemMeasurement* rowMeasurement = _rowMeasurements->at(x);
							if (!isnan(rowMeasurement->assigned_size))
							{
								currentOccupiedHeight += rowMeasurement->assigned_size;
							}
						}

						double currentFixedHeight = 0;
						int lastAutoRow = -1;
						if (_rowDefinitions->count() > 0)
						{
							for (int x = rowValue; x <= lastRow; x++)
							{
								ItemMeasurement* rowMeasurement = _rowMeasurements->at(x);
								RowDefinition* rowDefinition = _rowDefinitions->get_item(x);
								switch (rowDefinition->get_height().gridUnitType)
								{
								case GridUnitType::Auto:
									lastAutoRow = x;
									break;
								default:
									if (!isnan(rowMeasurement->assigned_size))
									{
										currentFixedHeight += rowMeasurement->assigned_size;
									}
									break;
								}
							}
						}

						void* columnPointer = uiElement->get_value(columnProperty);
						int columnValue = columnPointer == NULL ? 0 : *(int*)columnPointer;
						void* columnSpanPointer = uiElement->get_value(columnSpanProperty);
						int columnSpanValue = columnSpanPointer == NULL ? 1 : *(int*)columnSpanPointer;
						int lastColumn = columnValue + columnSpanValue - 1;

						double currentOccupiedWidth = 0;
						for (int x = 0; x < _columnDefinitions->count(); x++)
						{
							if (x >= columnValue || x <= lastColumn)
							{
								continue;
							}
							ItemMeasurement* columnMeasurement = _columnMeasurements->at(x);
							if (!isnan(columnMeasurement->assigned_size))
							{
								currentOccupiedWidth += columnMeasurement->assigned_size;
							}
						}

						double currentFixedWidth = 0;
						int lastAutoColumn = -1;

						if (_columnDefinitions->count() > 0)
						{
							for (int x = columnValue; x <= lastColumn; x++)
							{
								ColumnDefinition* columnDefinition = _columnDefinitions->get_item(x);
								ItemMeasurement* columnMeasurement = _columnMeasurements->at(x);
								switch (columnDefinition->get_width().gridUnitType)
								{
								case GridUnitType::Auto:
									lastAutoColumn = x;
									break;
								default:
									if (!isnan(columnMeasurement->assigned_size))
									{
										currentFixedWidth += columnMeasurement->assigned_size;
									}
									break;
								}
							}
						}

						if (lastAutoRow >= 0 || lastAutoColumn >= 0)
						{
							ItemMeasurement* rowMeasurement = lastAutoRow < 0 ? NULL : _rowMeasurements->at(lastAutoRow);
							ItemMeasurement* columnMeasurement = lastAutoColumn < 0 ? NULL : _columnMeasurements->at(lastAutoColumn);
							Size elementAvailableSize = Size();
							elementAvailableSize.height = lastAutoRow < 0 ? currentFixedHeight : availableSize.height - currentOccupiedHeight;
							elementAvailableSize.width = lastAutoColumn < 0 ? currentFixedWidth : availableSize.width - currentOccupiedWidth;
							uiElement->measure(elementAvailableSize);

							double heightDifference = uiElement->desiredSize.height - currentFixedHeight;
							if (heightDifference > 0)
							{
								rowMeasurement->measured_size = isnan(rowMeasurement->measured_size) ? heightDifference : rowMeasurement->measured_size + heightDifference;
							}
							double widthDifference = uiElement->desiredSize.width - currentFixedWidth;
							if (widthDifference > 0)
							{
								columnMeasurement->measured_size = isnan(columnMeasurement->measured_size) ? widthDifference : columnMeasurement->measured_size + widthDifference;
							}
						}
					}

					double measuredHeight = 0;
					for (int i = 0; i < rowCount; i++)
					{
						if (!isnan(_rowMeasurements->at(i)->measured_size))
						{
							measuredHeight += _rowMeasurements->at(i)->measured_size;
						}
					}

					double measuredWidth = 0;
					for (int i = 0; i < _columnDefinitions->count(); i++)
					{
						if (!isnan(_columnMeasurements->at(i)->measured_size))
						{
							measuredWidth += _columnMeasurements->at(i)->measured_size;
						}
					}

					double dynamicRowHeight = rowMultipliers > 0 ? (availableSize.height - measuredHeight) / rowMultipliers : 0;
					for (int i = 0; i < _rowDefinitions->count(); i++)
					{
						if (_rowDefinitions->get_item(i)->get_height_pointer()->gridUnitType == GridUnitType::Star)
						{
							_rowMeasurements->at(i)->measured_size = dynamicRowHeight;
						}
					}

					double dynamicColumnWidth = columnMultipliers > 0 ? (availableSize.width - measuredWidth) / columnMultipliers : 0;
					for (int i = 0; i < _columnDefinitions->count(); i++)
					{
						if (_columnDefinitions->get_item(i)->get_width_pointer()->gridUnitType == GridUnitType::Star)
						{
							_columnMeasurements->at(i)->measured_size = dynamicColumnWidth;
						}
					}
					
					for (int i = 0; i < childCount; i++)
					{
						UIElement* uiElement = get_children()->get_item(i);

						void* rowPointer = uiElement->get_value(rowProperty);
						int rowValue = rowPointer == NULL ? 0 : *(int*)rowPointer;
						void* rowSpanPointer = uiElement->get_value(rowSpanProperty);
						int rowSpanValue = rowSpanPointer == NULL ? 1 : *(int*)rowSpanPointer;
						int lastRow = rowValue + rowSpanValue - 1;

						void* columnPointer = uiElement->get_value(columnProperty);
						int columnValue = columnPointer == NULL ? 0 : *(int*)columnPointer;
						void* columnSpanPointer = uiElement->get_value(columnSpanProperty);
						int columnSpanValue = columnSpanPointer == NULL ? 1 : *(int*)columnSpanPointer;
						int lastColumn = columnValue + columnSpanValue - 1;

						bool hasAuto = false;
						double currentFixedHeight = 0;
						for (int x = rowValue; x <= lastRow; x++)
						{
							ItemMeasurement* rowMeasurement = _rowMeasurements->at(x);
							if (x < _rowDefinitions->count())
							{
								RowDefinition* rowDefinition = _rowDefinitions->get_item(x);
								switch (rowDefinition->get_height_pointer()->gridUnitType)
								{
								case GridUnitType::Star:
									currentFixedHeight += _rowMeasurements->at(x)->measured_size = dynamicRowHeight * (int)rowDefinition->get_height_pointer()->value;
									break;
								case GridUnitType::Pixel:
									currentFixedHeight += rowMeasurement->measured_size;
									break;
								case GridUnitType::Auto:
									hasAuto = true;
									break;
								}
								if (hasAuto)
								{
									break;
								}
							}
							else
							{
								currentFixedHeight += _rowMeasurements->at(x)->measured_size = dynamicRowHeight;
							}
						}
						if (hasAuto)
						{
							continue;
						}
						double currentFixedWidth = 0;
						for (int x = columnValue; x <= lastColumn; x++)
						{
							ItemMeasurement* columnMeasurement = _columnMeasurements->at(x);
							if (x < _columnDefinitions->count())
							{
								ColumnDefinition* columnDefinition = _columnDefinitions->get_item(x);
								switch (columnDefinition->get_width_pointer()->gridUnitType)
								{
								case GridUnitType::Star:
									currentFixedWidth += _columnMeasurements->at(x)->measured_size = dynamicColumnWidth * (int)columnDefinition->get_width_pointer()->value;
									break;
								case GridUnitType::Pixel:
									currentFixedWidth += columnMeasurement->measured_size;
									break;
								case GridUnitType::Auto:
									hasAuto = true;
									break;
								}
							}
							else
							{
								currentFixedWidth += _columnMeasurements->at(x)->measured_size = dynamicColumnWidth;
							}
						}
						if (hasAuto)
						{
							continue;
						}

						Size elementAvailableSize = Size();
						elementAvailableSize.width = currentFixedWidth;
						elementAvailableSize.height = currentFixedHeight;

						uiElement->measure(elementAvailableSize);
					}

					return availableSize;
				}

				Size arrangeOverride(Size finalSize)
				{

					XamlType* gridElementType = services->commonTypes->Xaml_UI_Grid();
					XamlAttachedProperty* rowProperty = services->commonTypes->Xaml_UI_Grid_Row();
					XamlAttachedProperty* rowSpanProperty = services->commonTypes->Xaml_UI_Grid_RowSpan();
					XamlAttachedProperty* columnProperty = services->commonTypes->Xaml_UI_Grid_Column();
					XamlAttachedProperty* columnSpanProperty = services->commonTypes->Xaml_UI_Grid_ColumnSpan();
					XamlType* frameworkElementType = services->commonTypes->Xaml_UI_FrameworkElement();

					int childCount = get_children()->count();
					for (int i = 0; i < childCount; i++)
					{
						UIElement* uiElement = get_children()->get_item(i);

						void* rowPointer = uiElement->get_value(rowProperty);
						int rowValue = rowPointer == NULL ? 0 : *(int*)rowPointer;
						void* rowSpanPointer = uiElement->get_value(rowSpanProperty);
						int rowSpanValue = rowSpanPointer == NULL ? 1 : *(int*)rowSpanPointer;
						void* columnPointer = uiElement->get_value(columnProperty);
						int columnValue = columnPointer == NULL ? 0 : *(int*)columnPointer;
						void* columnSpanPointer = uiElement->get_value(columnSpanProperty);
						int columnSpanValue = columnSpanPointer == NULL ? 1 : *(int*)columnSpanPointer;

						double yOffset = 0;
						for (int x = 0; x < rowValue; x++)
						{
							yOffset += _rowMeasurements->at(x)->measured_size;
						}

						double xOffset = 0;
						for (int x = 0; x < columnValue; x++)
						{
							xOffset += _columnMeasurements->at(x)->measured_size;
						}

						double rowsHeight = 0;
						int endRow = rowValue + rowSpanValue - 1;
						for (int x = rowValue; x <= endRow; x++)
						{
							ItemMeasurement* rowMeasurement = _rowMeasurements->at(x);
							rowsHeight += rowMeasurement->measured_size;
						}

						double columnsWidth = 0;
						int endColumn = columnValue + columnSpanValue - 1;
						for (int x = columnValue; x <= endColumn; x++)
						{
							ItemMeasurement* columnMeasurement = _columnMeasurements->at(x);
							columnsWidth += columnMeasurement->measured_size;
						}

						double targetHeight = uiElement->desiredSize.height;
						double targetWidth = uiElement->desiredSize.width;

						if (services->typeResolver->get_isSubtypeOf(uiElement->get_type(), frameworkElementType))
						{
							switch (((FrameworkElement*)uiElement)->get_verticalAlignment())
							{
							case VerticalAlignment::Bottom:
								yOffset += rowsHeight - targetHeight;
								break;
							case VerticalAlignment::Center:
								yOffset += (rowsHeight - targetHeight) / 2;
								break;
							case VerticalAlignment::Stretch:
								targetHeight = rowsHeight;
								break;
							default:
								break;
							}
							switch (((FrameworkElement*)uiElement)->get_horizontalAlignment())
							{
							case HorizontalAlignment::Right:
								xOffset += columnsWidth - targetWidth;
								break;
							case HorizontalAlignment::Center:
								xOffset += (columnsWidth - targetWidth) / 2;
								break;
							case HorizontalAlignment::Stretch:
								targetWidth = columnsWidth;
								break;
							default:
								break;
							}
						}

						Rect childRect;
						childRect.x = xOffset;
						childRect.y = yOffset;
						childRect.width = targetWidth;
						childRect.height = targetHeight;
						uiElement->arrange(childRect);
					}
					return finalSize;
				}

			public:
				Grid(XamlType* type, XamlServices* services) : Panel(type, services)
				{
					XamlType* rowDefinitionCollectionType = services->commonTypes->Xaml_UI_RowDefinitionCollection();
					XamlType* columnDefinitionCollectionType = services->commonTypes->Xaml_UI_ColumnDefinitionCollection();
					_rowDefinitions = new RowDefinitionCollection(rowDefinitionCollectionType,
						[=]() -> void
					{
						this->invalidateVisual();
					});
					_columnDefinitions = new ColumnDefinitionCollection(columnDefinitionCollectionType,
						[=]() -> void
					{
						this->invalidateVisual();
					});
				}

				RowDefinitionCollection* get_rowDefinitions()
				{
					return _rowDefinitions;
				}

				ColumnDefinitionCollection* get_columnDefinitions()
				{
					return _columnDefinitions;
				}

			};

		}
	}
}