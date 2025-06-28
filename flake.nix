{
  description = "dwm";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    dmenu = {
      url = "github:mow44/dmenu/main";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    scripts = {
      url = "path:/home/a/NixOS/scripts";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        flake-utils.follows = "flake-utils";
        dmenu.follows = "dmenu";
      };
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      dmenu,
      scripts,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        configFile = import ./config.nix {
          inherit pkgs scripts;
          dmenu = dmenu.packages.${system}.default;
        };
      in
      {
        packages = rec {
          dwm = pkgs.stdenv.mkDerivation {
            name = "dwm";
            version = "6.5";

            src = self;

            buildInputs = with pkgs; [
              xorg.libX11
              xorg.libXinerama
              xorg.libXft
            ];

            makeFlags = [ "CC=${pkgs.stdenv.cc.targetPrefix}cc" ];

            prePatch = ''
              sed -i "s@/usr/local@$out@" config.mk
              cp ${configFile} config.def.h
            '';

            meta = {
              mainProgram = "dwm";
            };
          };

          default = dwm;
        };
      }
    );
}
