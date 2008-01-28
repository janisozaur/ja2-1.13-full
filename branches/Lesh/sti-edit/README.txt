GIMP plugin for loading and saving files in STI format, used in game Jagged Alliance 2.

Read INSTALL.txt for install procedure.

This plugin allows:
- loading and saving both 8 and 16-bit sti files in GIMP

With this plugin you surely can't:
- save correct animation sti, made from the scratch
- save correct animation sti with new frames inserted inside existing sti
(both problems related to that fact, the animation sti has per-frame
non-video data)

You may have problems:
- while creating sti from the scratch, when subimages has offsets

What it lacks to be v1.0:
- per-frame non-video data editor for animation sti's
- progress-bars

Author: Lesh (Kouznetsov Alexei in real life)
        leshmail@list.ru
