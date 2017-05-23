/* this whole thing is a hack */

type drawComandT = unit => unit;

let create: Document.element => unit = [%bs.raw {|
  function(canvas) {
    window.regl = require('regl')(canvas);
  }
|}];

let makeDrawCommand: Obj.t => drawComandT = [%bs.raw {|
  function(drawCommand) {
    return window.regl(drawCommand);
  }
|}];

let draw: command::drawComandT => uniforms::Obj.t => unit = [%bs.raw {|
  function(drawCommand, uniforms) {
    return drawCommand(uniforms);
  }
|}];

let prop: string => unit = [%bs.raw {|
  function(key) {
    return window.regl.prop(key);
  }
|}];

let frame: (float => unit) => unit = [%bs.raw {|
  function(fn) {
    window.regl.frame((props) => fn(props.time));
  }
|}];

let clear: color::array int => depth::int => unit = [%bs.raw {|
  function(color, depth) {
    window.regl.clear({
      color: color,
      depth: depth
    });
  }
|}];

/* because we can't have nice things */

let unrollUniformForProp: string => int => unit = [%bs.raw {|
  function(key, num) {
    var uniform = {};

    for (var i = 0; i < num; ++i) {
      uniform[key + "[" + i + "]"] = window.regl.prop(key + "[" + i + "]");
    }

    return uniform;
  }

|}]
