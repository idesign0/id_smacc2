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

#include <smacc2/smacc_tracing/smacc_tracing.hpp>

#ifdef TRACETOOLS_LTTNG_ENABLED
#include <smacc2/smacc_tracing/trace_provider.hpp>
#define CONDITIONAL_TP(...) tracepoint(TRACEPOINT_PROVIDER, __VA_ARGS__)
#else
#define CONDITIONAL_TP(...)
#endif

// When tracetools is built with tracing DISABLED (TRACETOOLS_DISABLED -- the case on macOS, where
// there is no LTTng), tracetools.h defines:
//     #define _DECLARE_TRACEPOINT(...)            // expands to NOTHING
//     #define TRACETOOLS_TRACEPOINT(...) ((void) (0))
// so the declarations in smacc_tracing.hpp disappear and every call site becomes a no-op -- which
// is the intended behaviour. The definitions below must disappear with them: TRACEPOINT() is
// shimmed to TRACETOOLS_TRACEPOINT(), so `void TRACEPOINT(spinOnce) { ... }` expands to
// `void ((void) (0)) { ... }`:
//     smacc_tracing.cpp:30:6: error: expected unqualified-id
// Nothing references these functions in that configuration (the call sites are no-ops and the
// declarations are gone), so compiling them out is complete and safe.
#ifndef TRACETOOLS_DISABLED


void TRACEPOINT(spinOnce) { CONDITIONAL_TP(spinOnce); }

void TRACEPOINT(smacc2_event, const char * event_type) { CONDITIONAL_TP(smacc2_event, event_type); }

void TRACEPOINT(smacc2_state_update_start, const char * updatable_element_name)
{
  CONDITIONAL_TP(smacc2_state_update_start, updatable_element_name);
}

void TRACEPOINT(smacc2_state_update_end, const char * updatable_element_name)
{
  CONDITIONAL_TP(smacc2_state_update_end, updatable_element_name);
}

void TRACEPOINT(smacc2_state_onRuntimeConfigure_start, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onRuntimeConfigure_start, state_name);
}

void TRACEPOINT(smacc2_state_onRuntimeConfigure_end, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onRuntimeConfigure_end, state_name);
}

void TRACEPOINT(smacc2_state_onEntry_start, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onEntry_start, state_name);
}

void TRACEPOINT(smacc2_state_onEntry_end, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onEntry_end, state_name);
}

void TRACEPOINT(smacc2_state_onExit_start, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onExit_start, state_name);
}

void TRACEPOINT(smacc2_state_onExit_end, const char * state_name)
{
  CONDITIONAL_TP(smacc2_state_onExit_end, state_name);
}

void TRACEPOINT(
  smacc2_client_behavior_on_entry_start, const char * state_name, const char * orthogonal_name,
  const char * client_behavior_name)
{
  CONDITIONAL_TP(
    smacc2_client_behavior_on_entry_start, state_name, orthogonal_name, client_behavior_name);
}

void TRACEPOINT(
  smacc2_client_behavior_on_entry_end, const char * state_name, const char * orthogonal_name,
  const char * client_behavior_name)
{
  CONDITIONAL_TP(
    smacc2_client_behavior_on_entry_end, state_name, orthogonal_name, client_behavior_name);
}

void TRACEPOINT(
  smacc2_client_behavior_on_exit_start, const char * state_name, const char * orthogonal_name,
  const char * client_behavior_name)
{
  CONDITIONAL_TP(
    smacc2_client_behavior_on_exit_start, state_name, orthogonal_name, client_behavior_name);
}

void TRACEPOINT(
  smacc2_client_behavior_on_exit_end, const char * state_name, const char * orthogonal_name,
  const char * client_behavior_name)
{
  CONDITIONAL_TP(
    smacc2_client_behavior_on_exit_end, state_name, orthogonal_name, client_behavior_name);
}

#endif  // TRACETOOLS_DISABLED
