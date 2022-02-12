{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages."${system}";
        # some symlink/relative path shenanigans happening 🤷
        esy = pkgs.writeShellScriptBin "esy" "${pkgs.nodePackages.esy}/lib/node_modules/.bin/esy $@";
      in
      rec {
        packages = {
          relog = pkgs.callPackage ./. { inherit pkgs; };
        };
        defaultPackage = packages.relog;
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            libsodium
            esy
            ocamlformat
            ocamlPackages.ocaml-lsp
            ocamlPackages.reason
            ocamlPackages.dune_2
          ];
        };
      }
    );
}
