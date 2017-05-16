/* this whole thing is a hack */

type drawConfigT;
type drawComandT = unit => unit;

let create: Document.element => unit = [%bs.raw
  "function(canvas) {
    window.regl = require('regl')(canvas);
  }"
];

let makeDrawCommand: drawConfigT => drawComandT = [%bs.raw
  "function(drawCommand) {
    return window.regl(drawCommand);
  }"
];

let draw: command::drawComandT => uniforms::Obj.t? => unit = [%bs.raw
  "function(drawCommand, uniforms) {
    return drawCommand(uniforms);
  }"
];

let prop: string => unit = [%bs.raw
  "function(key) {
    return window.regl.prop(key);
  }"
];

let frame: (float => unit) => unit = [%bs.raw
  "function(fn) {
    window.regl.frame((props) => fn(props.time));
  }"
];

let clear: color::array int => depth::int => unit = [%bs.raw
  "function(color, depth) {
    window.regl.clear({
      color: color,
      depth: depth
    });
  }"
];

external makeDrawConfig:
  frag::string => vert::string => attributes::'a? => uniforms::'b? => count::int? => drawConfigT =
  "" [@@bs.obj];

