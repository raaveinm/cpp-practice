#pragma once

#include <iostream>
#include <ranges>
#include <string_view>

#include "budget_manager.h"

void ParseAndProcessQuery(BudgetManager& manager, std::string_view line);
int ReadNumberOnLine(std::istream& input);
