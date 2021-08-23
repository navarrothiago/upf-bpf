################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
# include_directories(${SRC_TOP_DIR}/common/utils)
include_directories(${SRC_TOP_DIR}/../build/ext/spdlog/include)


set(CN_UTILS_SRC STATIC
    # ${CMAKE_CURRENT_SOURCE_DIR}/3gpp_conversions.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/async_shell_cmd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/conversions.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/epc.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/get_gateway_netlink.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/if.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/pid_file.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/string.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/thread_sched.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/fqdn.cpp
    )


add_library(CN_UTILS ${CN_UTILS_SRC})


install(
    TARGETS CN_UTILS
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

