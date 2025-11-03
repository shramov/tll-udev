UDev Channel for TLL framework
==============================

TLL_ channel that reads udev events and wraps them in structure described in `Data Scheme`_ section.
Events can be filtered by subsystem using ``subsystem=<name>`` init parameter, for example watching
for power supply events: ``udev://;subsystem=power_supply`` or when using ``tll-read`` utility::

  tll-read --poll -m tll-udev 'udev://;subsystem=power_supply'

Compilation
-----------

Module depends on TLL_, ``libudev-dev``, ``libfmt-dev`` and uses meson_ build system (on non-Debian
systems names can differ). Compilation is straightforward::

  meson setup build
  ninja -vC build

Data Scheme
-----------

::

  - name: Property
    fields:
      - {name: name, type: string}
      - {name: value, type: string}

  - name: Device
    id: 10
    enums:
      Action: {type: int8, enum: { Unknown: 0, Add: 1, Bind: 2, Change: 3, Unbind: 4, Remove: 5 }}
    fields:
      - {name: action, type: Action}
      - {name: subsystem, type: string}
      - {name: sysname, type: string}
      - {name: devpath, type: string}
      - {name: properties, type: '*Property'}

.. _TLL: https://github.com/shramov/tll/
.. _meson: https://mesonbuild.com/ 

..
  vim: sts=2 sw=2 et tw=100
