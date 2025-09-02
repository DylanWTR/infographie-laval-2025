#pragma once

#include <functional>
#include <memory>
#include <string_view>

enum PropertyType {
    CATEGORY,
    POSITION,
    ROTATION,
    SCALE,
    TEXTURE,
    TEXTURE_ENABLED,
    SECTOR_COUNT,
    STACK_COUNT,
    COLOR,
    ROUGHNESS,
    METALLIC,
    FILTER,
};

class IProperty {
public:
    [[nodiscard]] virtual PropertyType type() const = 0;

    [[nodiscard]] virtual std::string_view name() const = 0;

    virtual ~IProperty() = default;
};

using PropertyPtr = std::shared_ptr<IProperty>;

class PropertyCategory final : public IProperty {
    std::string_view _name;

public:
    explicit PropertyCategory(const std::string_view &name) : _name(name) {
    }

    [[nodiscard]] PropertyType type() const override { return CATEGORY; }
    [[nodiscard]] std::string_view name() const override { return _name; }
};

template<typename T>
using UpdateValue = std::function<void(T)>;

template<typename T>
class Property final : public IProperty {
    PropertyType _type;
    std::string_view _name;
    T _value;
    UpdateValue<T> _updateValue;

public:
    explicit Property(const PropertyType type, const std::string_view &name, const T &defaultValue,
                      const UpdateValue<T> &updateValue)
        : _type(type),
          _name(name),
          _value(defaultValue),
          _updateValue(updateValue) {
    }

    [[nodiscard]] PropertyType type() const override { return _type; }

    [[nodiscard]] std::string_view name() const override { return _name; }

    [[nodiscard]] T value() const { return _value; }

    void updateValue(T value) {
        _value = value;
        _updateValue(value);
    }
};

using FloatProperty = Property<float>;
using IntProperty = Property<int>;
using BoolProperty = Property<bool>;
using StringProperty = Property<std::string>;

using FloatPropertyPtr = std::shared_ptr<FloatProperty>;
using IntPropertyPtr = std::shared_ptr<IntProperty>;
using BoolPropertyPtr = std::shared_ptr<BoolProperty>;
using StringPropertyPtr = std::shared_ptr<StringProperty>;
