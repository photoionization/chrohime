// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/yoga_util.h"

#include <algorithm>
#include <tuple>
#include <utility>

#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "yoga/Yoga.h"

namespace hime {

namespace {

// Converters to convert string to integer.
using IntConverter = bool(*)(std::u16string_view, int*);

bool AlignValue(std::u16string_view value, int* out) {
  if (value == u"auto")
    *out = static_cast<int>(YGAlignAuto);
  else if (value == u"base-line")
    *out = static_cast<int>(YGAlignBaseline);
  else if (value == u"space-between")
    *out = static_cast<int>(YGAlignSpaceBetween);
  else if (value == u"space-around")
    *out = static_cast<int>(YGAlignSpaceAround);
  else if (value == u"center")
    *out = static_cast<int>(YGAlignCenter);
  else if (value == u"stretch")
    *out = static_cast<int>(YGAlignStretch);
  else if (value == u"flex-start")
    *out = static_cast<int>(YGAlignFlexStart);
  else if (value == u"flex-end")
    *out = static_cast<int>(YGAlignFlexEnd);
  else
    return false;
  return true;
}

bool DirectionValue(std::u16string_view value, int* out) {
  if (value == u"ltr")
    *out = static_cast<int>(YGDirectionLTR);
  else if (value == u"rtl")
    *out = static_cast<int>(YGDirectionRTL);
  else if (value == u"inherit")
    *out = static_cast<int>(YGDirectionInherit);
  else
    return false;
  return true;
}

bool DisplayValue(std::u16string_view value, int* out) {
  if (value == u"flex")
    *out = static_cast<int>(YGDisplayFlex);
  else if (value == u"none")
    *out = static_cast<int>(YGDisplayNone);
  else
    return false;
  return true;
}

bool FlexDirectionValue(std::u16string_view value, int* out) {
  if (value == u"row")
    *out = static_cast<int>(YGFlexDirectionRow);
  else if (value == u"row-reverse")
    *out = static_cast<int>(YGFlexDirectionRowReverse);
  else if (value == u"column")
    *out = static_cast<int>(YGFlexDirectionColumn);
  else if (value == u"column-reverse")
    *out = static_cast<int>(YGFlexDirectionColumnReverse);
  else
    return false;
  return true;
}

bool JustifyValue(std::u16string_view value, int* out) {
  if (value == u"center")
    *out = static_cast<int>(YGJustifyCenter);
  else if (value == u"space-around")
    *out = static_cast<int>(YGJustifySpaceAround);
  else if (value == u"space-between")
    *out = static_cast<int>(YGJustifySpaceBetween);
  else if (value == u"space-evenly")
    *out = static_cast<int>(YGJustifySpaceEvenly);
  else if (value == u"flex-start")
    *out = static_cast<int>(YGJustifyFlexStart);
  else if (value == u"flex-end")
    *out = static_cast<int>(YGJustifyFlexEnd);
  else
    return false;
  return true;
}

bool OverflowValue(std::u16string_view value, int* out) {
  if (value == u"visible")
    *out = static_cast<int>(YGOverflowVisible);
  else if (value == u"hidden")
    *out = static_cast<int>(YGOverflowHidden);
  else
    return false;
  return true;
}

bool PositionValue(std::u16string_view value, int* out) {
  if (value == u"absolute")
    *out = static_cast<int>(YGPositionTypeAbsolute);
  else if (value == u"relative")
    *out = static_cast<int>(YGPositionTypeRelative);
  else
    return false;
  return true;
}

bool WrapValue(std::u16string_view value, int* out) {
  if (value == u"wrap")
    *out = static_cast<int>(YGWrapWrap);
  else if (value == u"nowrap")
    *out = static_cast<int>(YGWrapNoWrap);
  else if (value == u"wrap-reverse")
    *out = static_cast<int>(YGWrapWrapReverse);
  else
    return false;
  return true;
}

// Convert the value to pixel value.
float PixelValue(std::u16string_view value) {
  if (base::EndsWith(value, u"px", base::CompareCase::SENSITIVE))
    value = value.substr(0, value.length() - 2);
  double out;
  if (!base::StringToDouble(value, &out))
    return 0;
  return out;
}

// Convert the value to percent value.
int PercentValue(std::u16string_view value) {
  int integer;
  if (!base::StringToInt(value.substr(0, value.length() - 1), &integer))
    return 0;
  return integer;
}

// The gap APIs are not consistent, provide wrappers for them.
void YGNodeStyleSetGapAll(YGNodeRef node, float value) {
  YGNodeStyleSetGap(node, YGGutterAll, value);
}
void YGNodeStyleSetGapColumn(YGNodeRef node, float value) {
  YGNodeStyleSetGap(node, YGGutterColumn, value);
}
void YGNodeStyleSetGapRow(YGNodeRef node, float value) {
  YGNodeStyleSetGap(node, YGGutterRow, value);
}

// We use int to represent enums.
using IntSetter = void(*)(const YGNodeRef, int);
using FloatSetter = void(*)(const YGNodeRef, float);
using AutoSetter = void(*)(const YGNodeRef);
using EdgeSetter = void(*)(const YGNodeRef, const YGEdge, float);

// Sorted list of CSS node properties.
const std::tuple<const char16_t*, IntConverter, IntSetter> int_setters[] = {
  { u"aligncontent", AlignValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetAlignContent) },
  { u"alignitems", AlignValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetAlignItems) },
  { u"alignself", AlignValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetAlignSelf) },
  { u"direction", DirectionValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetDirection) },
  { u"display", DisplayValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetDisplay) },
  { u"flexdirection", FlexDirectionValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetFlexDirection) },
  { u"flexwrap", WrapValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetFlexWrap) },
  { u"justifycontent", JustifyValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetJustifyContent) },
  { u"overflow", OverflowValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetOverflow) },
  { u"position", PositionValue,
    reinterpret_cast<IntSetter>(YGNodeStyleSetPositionType) },
};
const std::pair<const char16_t*, FloatSetter> float_setters[] = {
  { u"aspectratio", YGNodeStyleSetAspectRatio },
  { u"columngap", YGNodeStyleSetGapColumn },
  { u"flex", YGNodeStyleSetFlex },
  { u"flexbasis", YGNodeStyleSetFlexBasis },
  { u"flexgrow", YGNodeStyleSetFlexGrow },
  { u"flexshrink", YGNodeStyleSetFlexShrink },
  { u"gap", YGNodeStyleSetGapAll },
  { u"height", YGNodeStyleSetHeight },
  { u"maxheight", YGNodeStyleSetMaxHeight },
  { u"maxwidth", YGNodeStyleSetMaxWidth },
  { u"minheight", YGNodeStyleSetMinHeight },
  { u"minwidth", YGNodeStyleSetMinWidth },
  { u"rowgap", YGNodeStyleSetGapRow },
  { u"width", YGNodeStyleSetWidth },
};
const std::pair<const char16_t*, AutoSetter> auto_setters[] = {
  { u"flexbasis", YGNodeStyleSetFlexBasisAuto},
  { u"height", YGNodeStyleSetHeightAuto},
  { u"width", YGNodeStyleSetWidthAuto},
};
const std::pair<const char16_t*, FloatSetter> percent_setters[] = {
  { u"flexbasis", YGNodeStyleSetFlexBasisPercent },
  { u"height", YGNodeStyleSetHeightPercent },
  { u"maxheight", YGNodeStyleSetMaxHeightPercent },
  { u"maxwidth", YGNodeStyleSetMaxWidthPercent },
  { u"minheight", YGNodeStyleSetMinHeightPercent },
  { u"minwidth", YGNodeStyleSetMinWidthPercent },
  { u"width", YGNodeStyleSetWidthPercent },
};
const std::tuple<const char16_t*, YGEdge, EdgeSetter> edge_setters[] = {
  { u"border", YGEdgeAll, YGNodeStyleSetBorder },
  { u"borderbottom", YGEdgeBottom, YGNodeStyleSetBorder },
  { u"borderleft", YGEdgeLeft, YGNodeStyleSetBorder },
  { u"borderright", YGEdgeRight, YGNodeStyleSetBorder },
  { u"bordertop", YGEdgeTop, YGNodeStyleSetBorder },
  { u"bottom", YGEdgeBottom, YGNodeStyleSetPosition },
  { u"left", YGEdgeLeft, YGNodeStyleSetPosition },
  { u"margin", YGEdgeAll, YGNodeStyleSetMargin },
  { u"marginbottom", YGEdgeBottom, YGNodeStyleSetMargin },
  { u"marginleft", YGEdgeLeft, YGNodeStyleSetMargin },
  { u"marginright", YGEdgeRight, YGNodeStyleSetMargin },
  { u"margintop", YGEdgeTop, YGNodeStyleSetMargin },
  { u"padding", YGEdgeAll, YGNodeStyleSetPadding },
  { u"paddingbottom", YGEdgeBottom, YGNodeStyleSetPadding },
  { u"paddingleft", YGEdgeLeft, YGNodeStyleSetPadding },
  { u"paddingright", YGEdgeRight, YGNodeStyleSetPadding },
  { u"paddingtop", YGEdgeTop, YGNodeStyleSetPadding },
  { u"right", YGEdgeRight, YGNodeStyleSetPosition },
  { u"top", YGEdgeTop, YGNodeStyleSetPosition },
};
const std::tuple<const char16_t*, YGEdge, EdgeSetter> edge_percent_setters[] = {
  { u"bottom", YGEdgeBottom, YGNodeStyleSetPositionPercent },
  { u"left", YGEdgeLeft, YGNodeStyleSetPositionPercent },
  { u"margin", YGEdgeAll, YGNodeStyleSetMarginPercent },
  { u"marginbottom", YGEdgeBottom, YGNodeStyleSetMarginPercent },
  { u"marginleft", YGEdgeLeft, YGNodeStyleSetMarginPercent },
  { u"marginright", YGEdgeRight, YGNodeStyleSetMarginPercent },
  { u"margintop", YGEdgeTop, YGNodeStyleSetMarginPercent },
  { u"padding", YGEdgeAll, YGNodeStyleSetPaddingPercent },
  { u"paddingbottom", YGEdgeBottom, YGNodeStyleSetPaddingPercent },
  { u"paddingleft", YGEdgeLeft, YGNodeStyleSetPaddingPercent },
  { u"paddingright", YGEdgeRight, YGNodeStyleSetPaddingPercent },
  { u"paddingtop", YGEdgeTop, YGNodeStyleSetPaddingPercent },
  { u"right", YGEdgeRight, YGNodeStyleSetPositionPercent },
  { u"top", YGEdgeTop, YGNodeStyleSetPositionPercent },
};

// Compare function to compare elements.
template<typename T>
bool ElementCompare(const T& e1, const T& e2) {
  return std::u16string_view(std::get<0>(e1)) < std::get<0>(e2);
}

// Check if the array is sorted.
template<typename T, size_t n>
bool IsSorted(T (&setters)[n]) {
  return std::is_sorted(std::begin(setters), std::end(setters),
                        ElementCompare<T>);
}

// Compare function to compare between elements and keys.
template<typename T>
bool FirstCompare(const T& element, std::u16string_view key) {
  return std::get<0>(element) < key;
}

// Find out the setter from array.
template<typename T, size_t n>
T* Find(T (&setters)[n], std::u16string_view name) {
  auto iter = std::lower_bound(std::begin(setters), std::end(setters), name,
                               FirstCompare<T>);
  if (iter == std::end(setters) || name != std::get<0>(*iter))
    return nullptr;
  return &(*iter);
}

// Set style for int properties.
bool SetIntStyle(YGNodeRef node,
                 std::u16string_view name,
                 std::u16string_view value) {
  auto* tup = Find(int_setters, name);
  if (!tup)
    return false;
  int converted;
  if (!std::get<1>(*tup)(value, &converted)) {
    LOG(WARNING) << "Invalid value " << value << " for property " << name;
    return false;
  }
  std::get<2>(*tup)(node, converted);
  return true;
}

// Set style for float properties.
bool SetFloatStyle(YGNodeRef node, std::u16string_view name, float value) {
  auto* tup = Find(float_setters, name);
  if (!tup)
    return false;
  std::get<1>(*tup)(node, value);
  return true;
}

// Set "auto" property for styles.
bool SetAutoStyle(YGNodeRef node, std::u16string_view name) {
  auto* tup = Find(auto_setters, name);
  if (!tup)
    return false;
  std::get<1>(*tup)(node);
  return true;
}

// Dispatch to float for auto depending on the value.
bool SetUnitStyle(YGNodeRef node,
                  std::u16string_view name,
                  std::u16string_view value) {
  if (value == u"auto")
    return SetAutoStyle(node, name);
  else
    return SetFloatStyle(node, name, PixelValue(value));
}

// Set style for percent properties.
bool SetPercentStyle(YGNodeRef node,
                     std::u16string_view name,
                     std::u16string_view value) {
  auto* tup = Find(percent_setters, name);
  if (!tup)
    return false;
  std::get<1>(*tup)(node, PercentValue(value));
  return true;
}

// Set style for edge properties.
bool SetEdgeStyle(YGNodeRef node, std::u16string_view name, float value) {
  auto* tup = Find(edge_setters, name);
  if (!tup)
    return false;
  std::get<2>(*tup)(node, std::get<1>(*tup), value);
  return true;
}

bool SetEdgeStyle(YGNodeRef node,
                  std::u16string_view name,
                  std::u16string_view value) {
  return SetEdgeStyle(node, name, PixelValue(value));
}

// Set style for edge percent properties.
bool SetEdgePercentStyle(YGNodeRef node,
                         std::u16string_view name,
                         std::u16string_view value) {
  auto* tup = Find(edge_percent_setters, name);
  if (!tup)
    return false;
  std::get<2>(*tup)(node, std::get<1>(*tup), PercentValue(value));
  return true;
}

// Check whether the value is xx%.
bool IsPercentValue(std::u16string_view value) {
  if (value.size() < 2 || value.size() > 4)
    return false;
  return value.back() == u'%';
}

}  // namespace

void SetYogaProperty(YGNodeRef node, std::u16string_view name, float value) {
  SetFloatStyle(node, name, value) ||
  SetEdgeStyle(node, name, value);
}

void SetYogaProperty(YGNodeRef node,
                     std::u16string_view name,
                     std::u16string_view value) {
  DCHECK(IsSorted(int_setters) &&
         IsSorted(float_setters) &&
         IsSorted(auto_setters) &&
         IsSorted(percent_setters) &&
         IsSorted(edge_setters) &&
         IsSorted(edge_percent_setters)) << "Property setters must be sorted";
  if (IsPercentValue(value)) {
    SetPercentStyle(node, name, value) ||
    SetEdgePercentStyle(node, name, value);
  } else {
    SetIntStyle(node, name, value) ||
    SetUnitStyle(node, name, value) ||
    SetEdgeStyle(node, name, value);
  }
}

}  // namespace hime
