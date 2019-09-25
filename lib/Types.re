type msgf('a) = (format4('a, Format.formatter, unit, unit) => 'a) => unit;

type log('a) = msgf('a) => unit;