# RC08B coding

## Frame format
Frame consist of 6 bytes, 3 containing data and 3 being their negation - in alternating order. Each byte stores 2 options - each in its own half byte. 

Structure of the frame:

``O`` ``R`` ``O``<sub>n</sub> ``R``<sub>n</sub> ``F`` ``S`` ``F``<sub>n</sub> ``S``<sub>n</sub> ``T`` ``M`` ``T``<sub>n</sub> ``M``<sub>n</sub>

Where:
* ``O`` - Other
* ``R`` - Turbo
* ``F`` - Fan
* ``S`` - Swing
* ``T`` - Temperature
* ``M`` - Mode
* *``X``*<sub>n</sub> - negated half byte

# Half byte values

## Temperature
|    Option | Value |
| --------: | :---: |
|        17 |   0   |
|        18 |   1   |
|        19 |   3   |
|        20 |   2   |
|        21 |   6   |
|        22 |   7   |
|        23 |   5   |
|        24 |   4   |
|        25 |  12   |
|        26 |  13   |
|        27 |   9   |
|        28 |   8   |
|  Turbo/29 |  10   |
|        30 |  11   |
| No change |  14   |

## Mode
|                 Option | Value |
| ---------------------: | :---: |
| Toggle swing / Cooling |   0   |
|           Turbo toggle |   2   |
| Fan / Dehumidification |   4   |
|                   Auto |   8   |
|                Heating |  12   |

## Fan
|                      Option | Value |
| --------------------------: | :---: |
| Dehumidification/ Auto Mode |   1   |
|                         Low |   9   |
|                      Medium |   5   |
|                        High |   3   |
|                        Auto |  11   |
|                Toggle swing |  12   |
|                       Turbo |  15   |
|                         Off |   7   |

## Swing
|       Option | Value |
| -----------: | :---: |
| Toggle swing |  11   |
| Toggle turbo |   5   |
|    No change |  15   |

## Turbo
|       Option | Value |
| -----------: | :---: |
| Toggle turbo |   5   |
|    No change |   2   |

## Other
|       Option | Value |
| -----------: | :---: |
| Toggle turbo |  11   |

# Usage

## Available options for Modes

|             Mode | Temperature |   Fan   |  Swing  |  Turbo  |
| ---------------: | :---------: | :-----: | :-----: | :-----: |
|             Auto |   &check;   | &cross; | &check; | &cross; |
|          Cooling |   &check;   | &check; | &check; | &check; |
|          Heating |   &check;   | &check; | &check; | &check; |
|              Fan |   &cross;   | &check; | &check; | &cross; |
| Dehumidification |   &check;   | &cross; | &check; | &cross; |

## Changing mode

|               Action |            Mode            |  Temperature  |                   Fan                   |       Swing       |      Turbo       |
| -------------------: | :------------------------: | :-----------: | :-------------------------------------: | :---------------: | :--------------: |
|             __Auto__ |          ``Auto``          | ``17``-``30`` |     ``Dehumidification/ Auto Mode``     |   ``No change``   |  ``No change``   |
|          __Cooling__ | ``Toggle swing / Cooling`` | ``17``-``30`` | ``Low``, ``Medium``, ``High``, ``Auto`` |   ``No change``   |  ``No change``   |
|          __Heating__ |        ``Heating``         | ``17``-``30`` | ``Low``, ``Medium``, ``High``, ``Auto`` |   ``No change``   |  ``No change``   |
|              __Fan__ | ``Fan / Dehumidification`` | ``No change`` | ``Low``, ``Medium``, ``High``, ``Auto`` |   ``No change``   |  ``No change``   |
| __Dehumidification__ | ``Fan / Dehumidification`` | ``17``-``30`` |    ``Dehumidification/ Auto Mode`` `    |   ``No change``   |  ``No change``   |
|     __Toggle turbo__ |       ``No change``        |    ``29``     |                ``Turbo``                | ``Toggle turbo``  | ``Toggle turbo`` |
|     __Toggle swing__ | ``Toggle swing / Cooling`` | ``No change`` |            ``Toggle swing``             | ``Toggle swing `` |  ``No change``   |
|         __Turn off__ | ``Toggle swing / Cooling`` | ``No change`` |                 ``Off``                 | ``Toggle swing``  |  ``No change``   |

### Turning on

To turn on the AC send any valid mode: ``Auto``, ``Cooling``, ``Heating`` , ``Fan`` or ``Dehumidification`` 