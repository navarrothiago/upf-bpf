/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */
#include "string.hpp"

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <stdarg.h>

template<class T>
class Buffer {
 public:
  explicit Buffer(size_t size) {
    msize = size;
    mbuf  = new T[msize];
  }
  ~Buffer() {
    if (mbuf) delete[] mbuf;
  }
  T* get() { return mbuf; }

 private:
  Buffer();
  size_t msize;
  T* mbuf;
};

std::string util::string_format(const char* format, ...) {
  va_list args;

  va_start(args, format);
  size_t size = vsnprintf(NULL, 0, format, args) + 1;  // Extra space for '\0'
  va_end(args);

  Buffer<char> buf(size);

  va_start(args, format);
  vsnprintf(buf.get(), size, format, args);
  va_end(args);

  return std::string(buf.get(), size - 1);  // We don't want the '\0' inside
}

// Licence : https://creativecommons.org/licenses/by-sa/4.0/legalcode
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring#217605

// trim from start
std::string& util::ltrim(std::string& s) {
  s.erase(
      s.begin(),
      std::find_if(
          s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
std::string& util::rtrim(std::string& s) {
  s.erase(
      std::find_if(
          s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)))
          .base(),
      s.end());
  return s;
}

// trim from both ends
std::string& util::trim(std::string& s) {
  return util::ltrim(util::rtrim(s));
}
