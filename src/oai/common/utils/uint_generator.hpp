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

/*! \file uint_uid_generator.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_UINT_GENERATOR_HPP_SEEN
#define FILE_UINT_GENERATOR_HPP_SEEN

#include <mutex>
#include <set>

namespace util {

template<class UINT>
class uint_generator {
 private:
  UINT uid_generator;
  std::mutex m_uid_generator;

  std::set<UINT> uid_generated;
  std::mutex m_uid_generated;

 public:
  uint_generator() : m_uid_generator(), m_uid_generated() {
    uid_generator = 0;
    uid_generated = {};
  };

  uint_generator(uint_generator const&) = delete;
  void operator=(uint_generator const&) = delete;

  UINT get_uid() {
    std::unique_lock<std::mutex> lr(m_uid_generator);
    UINT uid = ++uid_generator;
    while (true) {
      // may happen race conditions here
      std::unique_lock<std::mutex> ld(m_uid_generated);
      if (uid_generated.count(uid) == 0) {
        uid_generated.insert(uid);
        ld.unlock();
        lr.unlock();
        return uid;
      }
      uid = ++uid_generator;
    }
  }

  void free_uid(UINT uid) {
    std::unique_lock<std::mutex> l(m_uid_generated);
    uid_generated.erase(uid);
    l.unlock();
  }
};

template<class UINT>
class uint_uid_generator {
 private:
  UINT uid_generator;
  std::mutex m_uid_generator;

  std::set<UINT> uid_generated;
  std::mutex m_uid_generated;

  uint_uid_generator() : m_uid_generator(), m_uid_generated() {
    uid_generator = 0;
    uid_generated = {};
  };

 public:
  static uint_uid_generator& get_instance() {
    static uint_uid_generator instance;
    return instance;
  }

  uint_uid_generator(uint_uid_generator const&) = delete;
  void operator=(uint_uid_generator const&) = delete;

  UINT get_uid() {
    std::unique_lock<std::mutex> lr(m_uid_generator);
    UINT uid = ++uid_generator;
    while (true) {
      // may happen race conditions here
      std::unique_lock<std::mutex> ld(m_uid_generated);
      if (uid_generated.count(uid) == 0) {
        uid_generated.insert(uid);
        lr.unlock();
        ld.unlock();
        return uid;
      }
      uid = ++uid_generator;
    }
  }

  void free_uid(UINT uid) {
    std::unique_lock<std::mutex> l(m_uid_generated);
    uid_generated.erase(uid);
    l.unlock();
  }
};

}  // namespace util
#endif  // FILE_UINT_GENERATOR_HPP_SEEN
