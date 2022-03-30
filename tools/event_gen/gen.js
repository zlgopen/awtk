const fs = require('fs')
const path = require('path')

function genInitParamDecl(json) {
    return json.props.map(prop => {
        return `${prop.type} ${prop.name}`
    }).join(', ')
}


function genInitParamAssign(json) {
    return json.props.map(prop => {
        return `  event->${prop.name} = ${prop.name};`;
    }).join('\n')
}

function genImplOne(json) {
    let className = json.name;
    let uclassName = json.name.toUpperCase();

    let result = `
${className}_t* ${className}_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_${uclassName}, NULL);
  return_value_if_fail(event->size == sizeof(${className}_t), NULL);
 
  return (${className}_t*)event;
 }
 
event_t* ${className}_init(${className}_t* event, uint32_t type, void* target, ${genInitParamDecl(json)}) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(${className}_t));
 
  event->e = event_init(type, target);
  event->e.size = sizeof(*event);
${genInitParamAssign(json)}
 
  return (event_t*)event;
}    
`
    return result
}

function genImpl(json, name) {
    let result = `
/**
 * File:   ${name}_events.c
 * Author: AWTK Develop Team
 * Brief:  events structs
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * generate
 *
 */

#include "tkc.h"
#include "${name}_events.h"

`
    
    result += json.map(iter => {
        return genImplOne(iter)
    }).join('\n');

    return result;
}
function genPropsDecl(props) {
    const result = props.map(prop => {
        return `
  /** 
    * @property {${prop.type}} ${prop.name}
    * @annotation ["readable", "scriptable"]
    * ${prop.desc}
    */
    ${prop.type} ${prop.name};
`
    }).join('\n');

    return result;
}

function genInitParamComments(json) {
    return json.props.map(prop => {
        return ` * @param {${prop.type}} ${prop.name} ${prop.desc}`
    }).join('\n')
}

function genEventTypes(json) {
    return json.map((iter, i) => {
        const uname = iter.name.toUpperCase();
        const desc = iter.desc;
        const name = iter.name;

        return ` 
  /**  
    * @const EVT_${uname}
    * ${desc}(${name}_t)。
    */
    EVT_${uname} = EVT_USER_START + ${i}`
    }).join(',\n')
}

function genHeaderOne(json) {
    const className = json.name;
    const classDesc = json.desc;
    const uclassName = className.toUpperCase();
    const propsDecl = genPropsDecl(json.props);

    const result = `
/**
 * @class ${className}_t
 * @annotation ["scriptable"]
 * @parent event_t
 * ${classDesc}。
 */
typedef struct _${className}_t {
  event_t e;
${propsDecl}      
} ${className}_t;

/**
 * @method ${className}_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转${className}_t对象。
 * @param {event_t*} event event对象。
 *
 * @return {${className}_t*} event对象。
 */
${className}_t* ${className}_cast(event_t* event);

/**
 * @method ${className}_init
 * 初始化事件。
 * @param {${className}_t*} event event对象。
 * @param {void*} target 事件目标。
 * @param {uint32_t} type 事件类型。
${genInitParamComments(json)}
 *
 * @return {event_t*} event对象。
 */
event_t* ${className}_init(${className}_t* event, uint32_t type, void* target, ${genInitParamDecl(json)});

#define ${uclassName}(e) (${className}_t*)${className}_cast(e)
`

    return result;
}

function genHeader(json, name) {
    const uname = name.toUpperCase();
    const eventTypes = genEventTypes(json)
    let eventDecls = json.map(iter => {
        return genHeaderOne(iter)
    }).join('\n')

    let result = `
/**
 * File:   ${name}_events.h
 * Author: AWTK Develop Team
 * Brief:  events structs
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * generate
 *
 */

#ifndef TK_${uname}_EVENTS_H
#define TK_${uname}_EVENTS_H

#include "base/events.h"
        
BEGIN_C_DECLS

/**
  * @enum ${name}_event_type_t
  * @annotation ["scriptable"]
  * @prefix EVT_
  * 类型常量定义。
  */   
typedef enum _${name}_event_type_t {
${eventTypes}
} ${name}_event_type_t;

${eventDecls}   

END_C_DECLS

#endif/*${uname}_EVENTS_H*/
`

    return result;
}

function genWithJSON(json, name) {
    const h = genHeader(json, name)
    const c = genImpl(json, name)
    fs.writeFileSync(`${name}_events.h`, h)
    fs.writeFileSync(`${name}_events.c`, c)
}

function gen(filename) {
    const name = path.basename(filename, '.json');
    const json = JSON.parse(fs.readFileSync(filename).toString());
    genWithJSON(json, name);
}

if (process.argv.length != 3) {
    console.log(`Usage: node gen.js in.json`);
    process.exit(0);
} else {
    gen(process.argv[2]);
}

