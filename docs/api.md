# Vishay_VEML6035 library

## Methods

### `begin()`

Initialize the veml6035.

#### Syntax 

```
veml6035.begin()
```

#### Parameters

None.

#### Returns

1 on success, 0 on failure.

#### Example

```
if (!veml6035.begin()) {
    Serial.println("Failed to initialize veml6035!");
    while (1);
}
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_PRX()](#read_PRX)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [lens_factor](#lens_factor)

### `end()`

De-initialize the veml6035.

#### Syntax 

```
veml6035.end()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
if (!veml6035.begin()) {
    Serial.println("Failed to initialize veml6035!");
    while (1);
}

// Read data...

// Done with the veml6035 readings
veml6035.end();
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [lens_factor](#lens_factor)

### `read_ALS()`

Read the veml6035's Ambient Light sensor. 

#### Syntax 

```
veml6035.read_ALS()
```

#### Parameters

None.

#### Returns

Positive or zero on success, negative on failure.

#### Example

```
long als = veml6035.read_ALS();

if (als >= 0)
    Serial.print(als);

```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [lens_factor](#lens_factor)

### `get_lux()`

Read the veml6035's Ambient Lioght sensor and convert to lux. 

#### Syntax 

```
veml6035.get_lux()
```

#### Parameters

None.

#### Returns

Positive or zero on success, negative on failure.

#### Example

```
float lux = veml6035.get_lux();

if (lux >= 0)
    Serial.print(lux);

```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [lens_factor](#lens_factor)

### `lens_factor`

Lens Factor for customerize material effect. 

#### Syntax 

```
veml6035.lens_factor
```

#### Parameters

None.

#### Returns

Floating variable.

#### Example

```
Serial.print(veml6035.lens_factor);

```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [lens_factor](#lens_factor)
