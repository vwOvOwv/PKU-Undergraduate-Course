/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util/convert.hh"

#include <regex>

#include "simplessd/sim/trace.hh"

const std::regex regexInteger("(\\d+)([kKmMgGtTpP]?)",
                              std::regex_constants::ECMAScript);
const std::regex regexTime("(\\d+)([munp]?s?)",
                           std::regex_constants::ECMAScript |
                               std::regex_constants::icase);

bool convertBoolean(const char *value) {
  uint64_t number = convertInteger(value);

  if (number > 0) {
    return true;
  }

  if (strlen(value) == 1) {
    if (value[0] == 't' || value[0] == 'T' || value[0] == 'y' ||
        value[0] == 'Y') {
      return true;
    }
  }

  if (strcasecmp(value, "true") == 0 || strcasecmp(value, "yes") == 0) {
    return true;
  }

  return false;
}

uint64_t convertInteger(const char *value, bool *valid) {
  uint64_t ret = 0;
  std::cmatch match;

  if (valid) {
    *valid = false;
  }

  if (std::regex_match(value, match, regexInteger)) {
    if (valid) {
      *valid = true;
    }

    ret = strtoul(match[1].str().c_str(), nullptr, 10);

    if (match[2].length() > 0) {
      switch (match[2].str().at(0)) {
        case 'k':
          ret *= 1000ul;
          break;
        case 'K':
          ret *= 1024ul;
          break;
        case 'm':
          ret *= 1000000ul;
          break;
        case 'M':
          ret *= 1048576ul;
          break;
        case 'g':
          ret *= 1000000000ul;
          break;
        case 'G':
          ret *= 1073741824ul;
          break;
        case 't':
          ret *= 1000000000000ul;
          break;
        case 'T':
          ret *= 1099511627776ul;
          break;
        default:
          SimpleSSD::warn("Invalid suffix detected");
          break;
      }
    }
  }

  return ret;
}

uint64_t convertTime(const char *value, bool *valid) {
  uint64_t ret = 0;
  std::cmatch match;

  if (valid) {
    *valid = false;
  }

  if (std::regex_match(value, match, regexTime)) {
    if (valid) {
      *valid = true;
    }

    ret = strtoul(match[1].str().c_str(), nullptr, 10);

    if (match[2].length() > 0) {
      switch (match[2].str().at(0)) {
        case 's':
          ret *= 1000000000000ul;
          break;
        case 'm':
          ret *= 1000000000ul;
          break;
        case 'u':
          ret *= 1000000ul;
          break;
        case 'n':
          ret *= 1000ul;
          break;
        case 'p':
          // Do Nothing
          break;
        default:
          SimpleSSD::warn("Invalid suffix detected");
          break;
      }
    }
  }

  return ret;
}
