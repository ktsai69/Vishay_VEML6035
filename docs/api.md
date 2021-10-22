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
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
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
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `read_ALS()`

Read the veml6035's Ambient Light sensor count. 

#### Syntax 

```
boolean veml6035.read_ALS(uint16_t *als)
```

#### Parameters

als - point of uint16_t for als count

#### Returns

true on success, false on failure.

#### Example

```
uint16_t als;

if (veml6035.read_ALS(&als))
    Serial.print(als);

```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `get_lux()`

Read the veml6035's Ambient Lioght sensor and convert to lux. 

#### Syntax 

```
float veml6035.get_lux()
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
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `INT_EN()`

Enable/disable interrupt 

#### Syntax 

```
boolean veml6035.INT_EN()
```

#### Parameters

enable - true to enable interrupt, false to disable interrupt.

#### Returns

true on success, false on failure.

#### Example

```
// Enable interrupt
veml6035.INT_EN(enable);
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `INT_EN_with_threshold()`

Enable interrupt and set the threshold by percentage. 

#### Syntax 

```
boolean veml6035.INT_EN_with_threshold(float percent)
```

#### Parameters

percent - threshold percentage.

#### Returns

true on success, false on failure.

#### Example

```
// Enable interrupt and set threshold 3%
veml6035.INT_EN_with_threshold(3.0f);
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `read_INT_FLAG()`

Clean interrupt 

#### Syntax 

```
boolean veml6035.read_INT_FLAG(uint16_t *int_flag)
```

#### Parameters

int_flag - point of uint16_t.  The return values includes:
- **INT_FLAG_H** for excess the high threshold.
- **INT_FLAG_L** for below the low threshold.

#### Returns

true on success, false on failure.

#### Example

```
uint16_t int_flag;
if (veml6035.read_INT_FLAG(&int_flag))
{
  if (int_flag & veml6035.INT_FLAG_L)
    Serial.print("\tLow threshold.");
  if (int_flag & veml6035.INT_FLAG_H)
    Serial.print("\tHigh threshold.");
  Serial.println();
}
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [read_ALS()](#read_ALS)
* [get_lux()](#get_lux)
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)

### `lens_factor`

Lens Factor for customerize material effect. 

#### Syntax 

```
float veml6035.lens_factor
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
* [INT_EN()](#INT_EN)
* [INT_EN_with_threshold()](#INT_EN_with_threshold)
* [read_INT_FLAG()](#read_INT_FLAG)
* [lens_factor](#lens_factor)
