// Copyright 2021 RobosoftAI Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*****************************************************************************************************************
 *
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#include <tracetools/tracetools.h>

// Recent tracetools renamed the public DECLARE_TRACEPOINT() macro to the internal
// _DECLARE_TRACEPOINT() and, unlike TRACEPOINT()/DO_TRACEPOINT(), did NOT keep a deprecated
// DECLARE_TRACEPOINT alias -> "unknown type name 'DECLARE_TRACEPOINT'". Provide a compat shim so
// these tracepoint declarations build against both old and new tracetools. (_DECLARE_TRACEPOINT
// is a no-op when tracing is disabled, e.g. no LTTng on macOS.)
#ifndef DECLARE_TRACEPOINT
#define DECLARE_TRACEPOINT(...) _DECLARE_TRACEPOINT(__VA_ARGS__)
#endif
// The same tracetools cleanup also removed the deprecated TRACEPOINT()/DO_TRACEPOINT() aliases
// (renamed TRACETOOLS_TRACEPOINT()/TRACETOOLS_DO_TRACEPOINT()). smacc2 still calls the short
// names (e.g. TRACEPOINT(smacc2_event, ...)); without them the call sites see an undefined macro
// and fail with "use of undeclared identifier 'smacc2_event'". Shim them to the current names
// (both are no-ops when tracepoints are excluded, e.g. no LTTng on macOS).
#ifndef TRACEPOINT
#define TRACEPOINT(...) TRACETOOLS_TRACEPOINT(__VA_ARGS__)
#endif
#ifndef DO_TRACEPOINT
#define DO_TRACEPOINT(...) TRACETOOLS_DO_TRACEPOINT(__VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
  DECLARE_TRACEPOINT(spinOnce)

  DECLARE_TRACEPOINT(smacc2_event, const char * event_type)

  DECLARE_TRACEPOINT(smacc2_state_update_start, const char * updatable_element_name)

  DECLARE_TRACEPOINT(smacc2_state_update_end, const char * updatable_element_name)

  DECLARE_TRACEPOINT(smacc2_state_onRuntimeConfigure_start, const char * state_name)

  DECLARE_TRACEPOINT(smacc2_state_onRuntimeConfigure_end, const char * state_name)

  DECLARE_TRACEPOINT(smacc2_state_onEntry_start, const char * state_name)

  DECLARE_TRACEPOINT(smacc2_state_onEntry_end, const char * state_name)

  DECLARE_TRACEPOINT(smacc2_state_onExit_start, const char * state_name)

  DECLARE_TRACEPOINT(smacc2_state_onExit_end, const char * state_name)

  DECLARE_TRACEPOINT(
    smacc2_client_behavior_on_entry_start, const char * state_name, const char * orthogonal_name,
    const char * client_behavior_name)

  DECLARE_TRACEPOINT(
    smacc2_client_behavior_on_entry_end, const char * state_name, const char * orthogonal_name,
    const char * client_behavior_name)

  DECLARE_TRACEPOINT(
    smacc2_client_behavior_on_exit_start, const char * state_name, const char * orthogonal_name,
    const char * client_behavior_name)

  DECLARE_TRACEPOINT(
    smacc2_client_behavior_on_exit_end, const char * state_name, const char * orthogonal_name,
    const char * client_behavior_name)

#ifdef __cplusplus
}
#endif
