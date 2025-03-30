#ifndef COMBOBOXDATA_H
#define COMBOBOXDATA_H

/******************************************************************************
 * ComboBoxData.h
 *
 * This header file defines the typedef "ComboBoxData", which associates
 * a string (displayed in a combo box interface element), with the actual,
 * numeric value that will get selected when clicking on said option.
 *****************************************************************************/

#include <map>
#include <vector>
#include <string>

namespace Ui {

typedef std::vector<std::map<std::string, int>> ComboBoxData;

}

#endif
