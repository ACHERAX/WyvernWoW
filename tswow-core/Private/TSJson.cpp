/*
 * This file is part of tswow (https://github.com/tswow/).
 * Copyright (C) 2021 tswow <https://github.com/tswow/>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include "TSJson.h"
#include <nlohmann/json.hpp>

/*
 * JsonTag
 */

JsonTag::JsonTag(JsonType type, JsonValue value)
    : m_type(type), m_value(value) {}

JsonTag::JsonTag()
    : m_type(JsonType::NULL_LITERAL) {}

static void writeArray(TSJsonArray self, nlohmann::json& json);
static void readArray(TSJsonArray self, nlohmann::json const& source);
static void writeObject(TSJsonObject self, nlohmann::json& target);
static void readObject(TSJsonObject self, nlohmann::json const& source);

static void writeArray(TSJsonArray self, nlohmann::json& json)
{
    for (auto& value : *self.m_tags)
    {
        switch (value.m_type)
        {
        case JsonType::NUMBER:
            json.push_back(std::get<double>(value.m_value));
            break;
        case JsonType::STRING:
            json.push_back(std::get<TSString>(value.m_value));
            break;
        case JsonType::BOOL:
            json.push_back(std::get<bool>(value.m_value));
            break;
        case JsonType::OBJECT:
        {
            auto obj = std::get<TSJsonObject>(value.m_value);
            nlohmann::json objJson = nlohmann::json::object();
            writeObject(obj, objJson);
            json.push_back(objJson);
            break;
        }
        case JsonType::LIST:
        {
            auto list = std::get<TSJsonArray>(value.m_value);
            nlohmann::json listJson = nlohmann::json::array();
            writeArray(list, listJson);
            json.push_back(listJson);
            break;
        }
        case JsonType::NULL_LITERAL:
            json.push_back(nullptr);
            break;
        }
    }
}
static void readArray(TSJsonArray self, nlohmann::json const& source)
{
    self.m_tags->clear();
    self.m_tags->reserve(source.size());
    for (unsigned i = 0; i < source.size(); ++i)
    {
        auto val = source[i];
        switch (val.type())
        {
        case nlohmann::json::value_t::number_float:
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
            self.PushNumber(val);
            break;
        case nlohmann::json::value_t::boolean:
            self.PushBool(val);
            break;
        case nlohmann::json::value_t::string:
            self.PushString(TSString(val));
            break;
        case nlohmann::json::value_t::object: {
            TSJsonObject obj;
            readObject(obj, val);
            self.PushJsonObject(obj);
            break;
        }
        case nlohmann::json::value_t::array: {
            TSJsonArray arr;
            readArray(arr, val);
            self.PushJsonArray(arr);
            break;
        }
        case nlohmann::json::value_t::null: {
            self.PushNull();
            break;
        }
        }
    }
}
static void writeObject(TSJsonObject self, nlohmann::json& target)
{
    for (auto const& pair : *self.m_tags)
    {
        switch (pair.second.m_type)
        {
        case JsonType::NUMBER:
            target[pair.first] = std::get<double>(pair.second.m_value);
            break;
        case JsonType::STRING:
            target[pair.first] = std::get<TSString>(pair.second.m_value);
            break;
        case JsonType::BOOL:
            target[pair.first] = std::get<bool>(pair.second.m_value);
            break;
        case JsonType::OBJECT:
        {
            auto obj = std::get<TSJsonObject>(pair.second.m_value);
            writeObject(obj, target[pair.first] = nlohmann::json::object());
            break;
        }
        case JsonType::LIST:
        {
            auto list = std::get<TSJsonArray>(pair.second.m_value);
            writeArray(list, target[pair.first] = nlohmann::json::array());
            break;
        }
        case JsonType::NULL_LITERAL:
            target[pair.first] = nullptr;
            break;
        }
    }
}
static void readObject(TSJsonObject self, nlohmann::json const& source)
{
    for (auto& pair : source.items())
    {
        switch (pair.value().type())
        {
        case nlohmann::json::value_t::number_float:
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
            self.SetNumber(TSString(pair.key()), pair.value());
            break;
        case nlohmann::json::value_t::boolean:
            self.SetBool(TSString(pair.key()), pair.value());
            break;
        case nlohmann::json::value_t::string:
            self.SetString(TSString(pair.key()), TSString(pair.value()));
            break;
        case nlohmann::json::value_t::object: {
            TSJsonObject obj;
            readObject(obj, pair.value());
            self.SetJsonObject(TSString(pair.key()), obj);
            break;
        }
        case nlohmann::json::value_t::array: {
            TSJsonArray arr;
            readArray(arr, pair.value());
            self.SetJsonArray(TSString(pair.key()), arr);
            break;
        }
        case nlohmann::json::value_t::null: {
            self.SetNull(TSString(pair.key()));
        }
        }
    }
}

/*
 * TSJsonObject
 */

TSJsonObject::TSJsonObject()
    : m_tags(std::make_shared<std::map<std::string, JsonTag>>())
{

}

TSJsonObject::TSJsonObject(TSJsonObject const& map)
    : m_tags(map.m_tags) {}

bool TSJsonObject::IsValid()
{
    return m_is_valid;
}

template <typename T>
T TSJsonObject::get(TSString key, JsonType type, T value)
{
    if (contains(key, type))
    {
        return std::get<T>((*m_tags)[key].m_value);
    }
    else
    {
        return value;
    }
}

template <typename T>
TSJsonObject TSJsonObject::set(TSString key, JsonType type, T value)
{
    (*m_tags)[key] = JsonTag(type, value);
    return *this;
}

bool TSJsonObject::contains(TSString key, JsonType type)
{
    return m_tags->find(key.std_str()) != m_tags->end()
        && (*m_tags)[key.std_str()].m_type == type;
}

TSJsonObject TSJsonObject::SetBool(TSString key, bool value)
{
    return set(key, JsonType::BOOL, value);
}
bool TSJsonObject::HasBool(TSString key)
{
    return contains(key, JsonType::BOOL);
}

bool TSJsonObject::GetBool(TSString key, bool def)
{
    return get(key, JsonType::BOOL, def);
}

TSJsonObject TSJsonObject::SetNumber(TSString key, double value)
{
    return set(key, JsonType::NUMBER, value);
}
bool TSJsonObject::HasNumber(TSString key)
{
    return contains(key, JsonType::NUMBER);
}

double TSJsonObject::GetNumber(TSString key, double def)
{
    return get(key, JsonType::NUMBER, def);
}

TSJsonObject TSJsonObject::SetString(TSString key, TSString value)
{
    return set(key, JsonType::STRING, value);
}
bool TSJsonObject::HasString(TSString key)
{
    return contains(key, JsonType::STRING);
}

TSString TSJsonObject::GetString(TSString key, TSString def)
{
    return get(key, JsonType::STRING, def);
}

TSJsonObject TSJsonObject::SetNull(TSString key)
{
    return set(key, JsonType::NULL_LITERAL, nullptr);
}

bool TSJsonObject::HasNull(TSString key)
{
    return contains(key, JsonType::NULL_LITERAL);
}

TSJsonObject TSJsonObject::SetJsonObject(TSString key, TSJsonObject value)
{
    return set(key, JsonType::OBJECT, value);
}

bool TSJsonObject::HasJsonObject(TSString key)
{
    return contains(key, JsonType::OBJECT);
}

TSJsonObject TSJsonObject::GetJsonObject(TSString key, TSJsonObject value)
{
    return get(key, JsonType::OBJECT, value);
}

TSJsonObject TSJsonObject::SetJsonArray(TSString key, TSJsonArray value = TSJsonArray())
{
    return set(key, JsonType::LIST, value);
}

bool TSJsonObject::HasJsonArray(TSString key)
{
    return contains(key, JsonType::LIST);
}

TSJsonArray TSJsonObject::GetJsonArray(TSString key, TSJsonArray value = TSJsonArray())
{
    return get(key, JsonType::LIST, value);
}

void TSJsonObject::Parse(TSString json)
{
    try {
        readObject(*this, nlohmann::json::parse(json.std_str()));
        m_is_valid = true;
    }
    catch (std::exception e)
    {
        m_is_valid = false;
    }
}

TSString TSJsonObject::toString(int indents)
{
    if (m_tags->size() == 0)
    {
        return JSTR("{}");
    }
    nlohmann::json json = nlohmann::json::object();
    writeObject(*this, json);
    return TSString(indents >= 0 ? json.dump(indents) : json.dump());
}

TSJsonObject TSJsonObject::Remove(TSString key)
{
    m_tags->erase(key.std_str());
    return *this;
}

unsigned TSJsonObject::get_length()
{
    return m_tags->size();
}

/*
 * TSJsonArray
 */

TSJsonArray::TSJsonArray()
    : m_tags(std::make_shared<std::vector<JsonTag>>()) {}

TSJsonArray::TSJsonArray(TSJsonArray const& arr)
    : m_tags(arr.m_tags) {}

bool TSJsonArray::isValid()
{
    return m_is_valid;
}

bool TSJsonArray::contains(unsigned key, JsonType type)
{
    return m_tags->size() > key && (*m_tags)[key].m_type == type;
}

template <typename T>
T TSJsonArray::get(unsigned key, JsonType type, T def)
{
    return contains(key, type) ? std::get<T>((*m_tags)[key].m_value) : def;
}

template <typename T>
TSJsonArray TSJsonArray::set(unsigned key, JsonType type, T value)
{
    if (key >= m_tags->size())
    {
        m_tags->resize(key + 1);
    }
    (*m_tags)[key] = JsonTag(type, value);
    return *this;
}

template <typename T>
TSJsonArray TSJsonArray::insert(unsigned key, JsonType type, T value)
{
    if (key >= m_tags->size())
    {
        m_tags->resize(key);
    }
    m_tags->insert(m_tags->begin() + key, JsonTag(type, value));
    return *this;
}

template <typename T>
TSJsonArray TSJsonArray::push(JsonType type, T value)
{
    m_tags->push_back(JsonTag(type, value));
    return *this;
}

TSJsonArray TSJsonArray::SetBool(unsigned key, bool value)
{
    return set(key, JsonType::BOOL, value);
}

bool TSJsonArray::HasBool(unsigned key)
{
    return contains(key, JsonType::BOOL);
}

bool TSJsonArray::GetBool(unsigned key, bool def)
{
    return get(key, JsonType::BOOL, def);
}

TSJsonArray TSJsonArray::InsertBool(unsigned key, bool value)
{
    return set(key, JsonType::BOOL, value);
}

TSJsonArray TSJsonArray::PushBool(bool value)
{
    return push(JsonType::BOOL, value);
}

TSJsonArray TSJsonArray::SetNumber(unsigned key, double value)
{
    return set(key, JsonType::NUMBER, value);
}

bool TSJsonArray::HasNumber(unsigned key)
{
    return contains(key, JsonType::NUMBER);
}

double TSJsonArray::GetNumber(unsigned key, double def)
{
    return get(key, JsonType::NUMBER, def);
}

TSJsonArray TSJsonArray::InsertNumber(unsigned key, double value)
{
    return insert(key, JsonType::NUMBER, value);
}

TSJsonArray TSJsonArray::PushNumber(double value)
{
    return push(JsonType::NUMBER, value);
}

TSJsonArray TSJsonArray::SetString(unsigned key, TSString value)
{
    return set(key, JsonType::STRING, value);
}

bool TSJsonArray::HasString(unsigned key)
{
    return contains(key, JsonType::STRING);
}

TSString TSJsonArray::GetString(unsigned key, TSString def)
{
    return get(key, JsonType::STRING, def);
}

TSJsonArray TSJsonArray::SetNull(unsigned key)
{
    return set(key, JsonType::NULL_LITERAL, nullptr);
}

bool TSJsonArray::HasNull(unsigned key)
{
    return contains(key, JsonType::NULL_LITERAL);
}

TSJsonArray TSJsonArray::InsertNull(unsigned key)
{
    return insert(key, JsonType::NULL_LITERAL, nullptr);
}

TSJsonArray TSJsonArray::PushNull()
{
    return push(JsonType::NULL_LITERAL, nullptr);
}

TSJsonArray TSJsonArray::InsertString(unsigned key, TSString value)
{
    return insert(key, JsonType::STRING, value);
}

TSJsonArray TSJsonArray::PushString(TSString value)
{
    return push(JsonType::STRING, value);
}

TSJsonArray TSJsonArray::SetJsonObject(unsigned key, TSJsonObject value)
{
    return set(key, JsonType::OBJECT, value);
}

bool TSJsonArray::HasJsonObject(unsigned key)
{
    return contains(key, JsonType::OBJECT);
}

TSJsonObject TSJsonArray::GetJsonObject(unsigned key, TSJsonObject value)
{
    return get(key, JsonType::OBJECT, value);
}

TSJsonArray TSJsonArray::InsertJsonObject(unsigned key, TSJsonObject value)
{
    return insert(key, JsonType::OBJECT, value);
}

TSJsonArray TSJsonArray::PushJsonObject(TSJsonObject value)
{
    return push(JsonType::OBJECT, value);
}

TSJsonArray TSJsonArray::SetJsonArray(unsigned key, TSJsonArray arr)
{
    return set(key, JsonType::LIST, arr);
}

bool TSJsonArray::HasJsonArray(unsigned key)
{
    return contains(key, JsonType::LIST);
}

TSJsonArray TSJsonArray::GetJsonArray(unsigned key, TSJsonArray value)
{
    return get(key, JsonType::LIST, value);
}

TSJsonArray TSJsonArray::InsertJsonArray(unsigned key, TSJsonArray value)
{
    return insert(key, JsonType::LIST, value);
}

TSJsonArray TSJsonArray::PushJsonArray(TSJsonArray value)
{
    return push(JsonType::LIST, value);
}

TSString TSJsonArray::toString(int indents)
{
    if (m_tags->size() == 0)
    {
        return JSTR("[]");
    }
    nlohmann::json json = nlohmann::json::array();
    writeArray(*this, json);
    return TSString(indents >= 0 ? json.dump(indents) : json.dump());
}

TSJsonArray TSJsonArray::Remove(unsigned key)
{
    m_tags->erase(m_tags->begin()+key);
    return *this;
}

void TSJsonArray::Parse(TSString json)
{
    try {
        readArray(*this, nlohmann::json::parse(json.std_str()));
        m_is_valid = true;
    }
    catch (std::exception e)
    {
        m_is_valid = false;
    }
}

unsigned TSJsonArray::get_length()
{
    return m_tags->size();
}

TSJsonObject TSJsonObject::LSetBool(std::string const& key, bool value)
{
    return SetBool(key, value);
}
bool TSJsonObject::LHasBool(std::string const& key)
{
    return HasBool(key);
}
bool TSJsonObject::LGetBool0(std::string const& key, bool def)
{
    return GetBool(key, def);
}
bool TSJsonObject::LGetBool1(std::string const& key)
{
    return GetBool(key);
}

TSJsonObject TSJsonObject::LSetNumber(std::string const& key, double number)
{
    return SetNumber(key, number);
}
bool TSJsonObject::LHasNumber(std::string const& key)
{
    return HasNumber(key);
}
double TSJsonObject::LGetNumber0(std::string const& key, double def)
{
    return GetNumber(key, def);
}
double TSJsonObject::LGetNumber1(std::string const& key)
{
    return GetNumber(key);
}

TSJsonObject TSJsonObject::LSetString(std::string const& key, std::string const& number)
{
    return SetString(key, number);
}
bool TSJsonObject::LHasString(std::string const& key)
{
    return HasString(key);
}
std::string TSJsonObject::LGetString0(std::string const& key, std::string const& def)
{
    return GetString(key, def);
}
std::string TSJsonObject::LGetString1(std::string const& key)
{
    return GetString(key);
}

TSJsonObject TSJsonObject::LSetNull(std::string const& key)
{
    return SetNull(key);
}
bool TSJsonObject::lHasNull(std::string const& key)
{
    return HasNull(key);
}

TSJsonObject TSJsonObject::LSetJsonObject(std::string const& key, TSJsonObject value)
{
    return SetJsonObject(key, value);
}
bool TSJsonObject::LHasJsonObject(std::string const& key)
{
    return HasJsonObject(key);
}
TSJsonObject TSJsonObject::LGetJsonObject0(std::string const& key, TSJsonObject value)
{
    return GetJsonObject(key, value);
}
TSJsonObject TSJsonObject::LGetJsonObject1(std::string const& key)
{
    return GetJsonObject(key);
}

TSJsonArray TSJsonObject::LGetJsonArray0(std::string const& key, TSJsonArray arr)
{
    return GetJsonArray(key, arr);
}
TSJsonArray TSJsonObject::LGetJsonArray1(std::string const& key)
{
    return GetJsonArray(key);
}
bool TSJsonObject::LHasJsonArray(std::string const& key)
{
    return HasJsonArray(key);
}
TSJsonObject TSJsonObject::LSetJsonArray(std::string const& key, TSJsonArray value)
{
    return SetJsonArray(key, value);
}
std::string TSJsonObject::LtoString0(int indents)
{
    return toString(indents);
}
std::string TSJsonObject::LtoString1()
{
    return toString();
}
TSJsonObject TSJsonObject::LRemove(std::string const& key)
{
    return Remove(key);
}

bool TSJsonArray::LGetBool0(unsigned key, bool def)
{
    return GetBool(key, def);
}
bool TSJsonArray::LGetBool1(unsigned key)
{
    return GetBool(key);
}
double TSJsonArray::LGetNumber0(unsigned key, double def)
{
    return GetNumber(key, def);
}
double TSJsonArray::LGetNumber1(unsigned key)
{
    return GetNumber(key);
}

std::string TSJsonArray::LGetString0(unsigned key, std::string const& def)
{
    return GetString(key, def);
}
std::string TSJsonArray::LGetString1(unsigned key)
{
    return GetString(key);
}

TSJsonArray TSJsonArray::LInsertString(unsigned key, std::string const& value)
{
    return InsertString(key, value);
}
TSJsonArray TSJsonArray::LPushString(std::string const& value)
{
    return PushString(value);
}

TSJsonArray TSJsonArray::LSetJsonObject0(unsigned key, TSJsonObject value)
{
    return SetJsonObject(key, value);
}
TSJsonArray TSJsonArray::LSetJsonObject1(unsigned key)
{
    return SetJsonObject(key);
}
TSJsonObject TSJsonArray::LGetJsonObject0(unsigned key, TSJsonObject value)
{
    return GetJsonObject(key, value);
}
TSJsonObject TSJsonArray::LGetJsonObject1(unsigned key)
{
    return GetJsonObject(key);
}
TSJsonArray TSJsonArray::LInsertJsonObject0(unsigned key, TSJsonObject value)
{
    return InsertJsonObject(key, value);
}
TSJsonArray TSJsonArray::LInsertJsonObject1(unsigned key)
{
    return InsertJsonObject(key);
}

TSJsonArray TSJsonArray::LPushJsonObject0(TSJsonObject value)
{
    return PushJsonObject(value);
}
TSJsonArray TSJsonArray::LPushJsonObject1()
{
    return PushJsonObject();
}

TSJsonArray TSJsonArray::LSetJsonArray0(unsigned key, TSJsonArray arr)
{
    return SetJsonArray(key, arr);
}
TSJsonArray TSJsonArray::LSetJsonArray1(unsigned key)
{
    return SetJsonArray(key);
}

TSJsonArray TSJsonArray::LGetJsonArray0(unsigned key, TSJsonArray value)
{
    return GetJsonArray(key, value);
}
TSJsonArray TSJsonArray::LGetJsonArray1(unsigned key)
{
    return GetJsonArray(key);
}
TSJsonArray TSJsonArray::LInsertJsonArray0(unsigned key, TSJsonArray value)
{
    return InsertJsonArray(key, value);
}
TSJsonArray TSJsonArray::LInsertJsonArray1(unsigned key)
{
    return InsertJsonArray(key);
}

TSJsonArray TSJsonArray::LPushJsonArray0(TSJsonArray value)
{
    return PushJsonArray(value);
}
TSJsonArray TSJsonArray::LPushJsonArray1()
{
    return PushJsonArray();
}

TSString TSJsonArray::LtoString0(int indents)
{
    return toString(indents);
}
TSString TSJsonArray::LtoString1()
{
    return toString();
}

TSJsonArray TSJsonArray::LSetString(unsigned key, std::string const& number)
{
    return SetString(key, number);
}

