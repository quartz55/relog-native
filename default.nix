with import <nixpkgs> {};
mkShell {
  buildInputs = [nodejs-11_x];
  shellHook = ''
    export PATH="`pwd`/node_modules/.bin:$PATH"
  '';
}
