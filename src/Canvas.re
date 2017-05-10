type context;

external getContext : Document.element => string => context = "getContext" [@@bs.send];

external beginPath : context => unit = "beginPath" [@@bs.send];
external fillStyle : context => string => unit = "fillStyle" [@@bs.set];
external strokeStyle : context => string => unit = "strokeStyle" [@@bs.set];
external fill : context => unit = "fill" [@@bs.send];
external stroke : context => unit = "stroke" [@@bs.send];

external fillRect : context => float => float => float => float => unit = "fillRect" [@@bs.send];
external clearRect : context => float => float => float => float => unit = "clearRect" [@@bs.send];
external arc : context => float => float => float => float => float => bool => unit = "arc" [@@bs.send];

type imageData;

external newImageData : TypedArray.uint8ClampedArray => int => int => imageData = "ImageData" [@@bs.new];
external putImageData : context => imageData => int => int => unit = "putImageData" [@@bs.send];
