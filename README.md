# yoshimi #

This repo contains a headless **yoshimi** soft synth for web browsers. Forked from upstream [yoshimi](https://github.com/Yoshimi/yoshimi), and ported to Web Assembly. My goal was to produce as minimal changes to the original codebase as possible, and manage the port using #ifdefs and stub header dependencies. KissFFT and mxml bitcode files are linked in from the build folder.

online [**demo**](https://webaudiomodules.org/demos/jariseon/yoshimi/)

## building on osx ##

```
cd wam/build
sh build.sh
```

The build script produces yoshimi.wasm and yoshimi.js files, and moves them to wam/dist/worklet folder. **yoshimi.wasm** is the web assembly binary, and **yoshimi.js** contains its supporting glue code. Build process relies on emscripten wasm [toolchain](https://webassembly.org/getting-started/developers-guide/). Be sure to add emscripten tools directory (i.e., the one containing `emmake`) to path before running the yoshimi build script.

## running ##

`wam/dist` folder contains a pre-compiled distribution. Simply copy the presets from repository root **banks** folder into **wam/dist/banks**, serve the files with a localhost, and browse to `wam/dist/index.html` for a sweet sonic experience.

## credits ##
- yoshimi / zynaddsubfx [contributors](https://github.com/Yoshimi/yoshimi/blob/master/Yoshimi_Helpers) : Paul Nasca, Alan Calvert, Will Godfrey and others
- [kissFFT](https://github.com/mborgerding/kissfft) : Mark Borgerding
- [mxml](https://github.com/michaelrsweet/mxml) : Copyright © 2003-2019 by Michael R Sweet
- [zlib.js](https://github.com/imaya/zlib.js) : Copyright © 2012 imaya
- [qwerty-hancock](http://stuartmemo.com/qwerty-hancock) : Copyright 2012-14 Stuart Memo
- [web audio modules (WAMs)](https://github.com/webaudiomodules) : Jari Kleimola and Oliver Larkin 2015-2019
- [audioworklet-polyfill](https://github.com/webaudiomodules) : Jari Kleimola 2017-2019

## license ##
Yoshimi is GPL-2.0, WAMs and audioworklet-polyfill are under MIT. Please click the links above for other library licenses.
