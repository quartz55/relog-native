type msgf('a) =
  (
    (~fields: list(Field.t)=?, format4('a, Format.formatter, unit, unit)) =>
    unit
  ) =>
  unit;

type log('a) = msgf('a) => unit;