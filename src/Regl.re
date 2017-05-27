/* this whole thing is a hack, but ok to use I think */

type drawComandT = unit => unit;
type drawConfigT;

let create : Document.element => unit = [%bs.raw {|
  function(canvas) {
    // first hacky thing, expose regl as top-level window things
    window.regl = require('regl')(canvas);
  }
|}];

/* creates JS object from config */
external makeDrawConfig :
  frag::string? =>
  vert::string? =>
  uniforms::Js.t{..}? =>
  attributes::Js.t{..}? =>
  count::int? =>
  drawConfigT = "" [@@bs.obj];

/* makes draw command from draw config */
let makeDrawCommand : drawConfigT => drawComandT = [%bs.raw {|
  function(opts) {
    var command = window.regl(opts);

    /* expose ".draw" so we can use extarnal draw easily */
    command.draw = command;

    return command;
  }
|}];

/* draws the draw command using hack introduced in makeDrawCommand */
external draw : command::drawComandT => uniforms::Js.t 'a? => unit = "" [@@bs.send];

let prop : string => unit = [%bs.raw {|
  function(key) {
    return window.regl.prop(key);
  }
|}];

let frame : (float => unit) => unit = [%bs.raw {|
  function(fn) {
    window.regl.frame((props) => fn(props.time));
  }
|}];

/* like this, so we can easily set color/depth */
let clear : color::array int => depth::int => unit = [%bs.raw {|
  function(color, depth) {
    window.regl.clear({
      color: color,
      depth: depth
    });
  }
|}];

/* uniform arrays are only supported through full path: "uniformArray[0]",
so we have to unroll them, and that's easier in JS */
let unrollUniformForProp : string => int => Js.t { .. } = [%bs.raw {|
  function(key, num) {
    var uniform = {};

    for (var i = 0; i < num; ++i) {
      uniform[key + "[" + i + "]"] = window.regl.prop(key + "[" + i + "]");
    }

    return uniform;
  }
|}]
