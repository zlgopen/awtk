/*
 * Copyright (C) 2014-2026 Muhammad Tayyab Akram
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <SheenBidi/SBConfig.h>
#include <SheenBidi/SheenBidi.h>

#ifdef SB_CONFIG_UNITY

#include <API/SBAlgorithm.c>
#include <API/SBAllocator.c>
#include <API/SBAttributeList.c>
#include <API/SBAttributeRegistry.c>
#include <API/SBBase.c>
#include <API/SBCodepoint.c>
#include <API/SBCodepointSequence.c>
#include <API/SBLine.c>
#include <API/SBLog.c>
#include <API/SBMirrorLocator.c>
#include <API/SBParagraph.c>
#include <API/SBScriptLocator.c>
#include <API/SBText.c>
#include <API/SBTextConfig.c>
#include <API/SBTextIterators.c>

#include <Core/List.c>
#include <Core/Memory.c>
#include <Core/Object.c>
#include <Core/Once.c>

#include <Data/BidiTypeLookup.c>
#include <Data/GeneralCategoryLookup.c>
#include <Data/PairingLookup.c>
#include <Data/ScriptLookup.c>

#include <Script/ScriptStack.c>

#include <Text/AttributeDictionary.c>
#include <Text/AttributeManager.c>

#include <UBA/BidiChain.c>
#include <UBA/BracketQueue.c>
#include <UBA/IsolatingRun.c>
#include <UBA/LevelRun.c>
#include <UBA/RunQueue.c>
#include <UBA/StatusStack.c>

#endif
