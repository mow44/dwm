{
  description = "dwm";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    scripts = {
      url = "path:/home/a/NixOS/scripts";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        flake-utils.follows = "flake-utils";
      };
    };

    dmenu = {
      url = "github:mow44/dmenu/main";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      scripts,
      dmenu,
      ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      configFile = import ./config.nix {
        inherit pkgs scripts;
        dmenu = dmenu.defaultPackage.x86_64-linux;
      };
    in
    {
      defaultPackage.${system} =
        with pkgs;
        stdenv.mkDerivation {
          name = "dwm";
          version = "6.5";

          src = self;

          buildInputs = with pkgs; [
            xorg.libX11
            xorg.libXinerama
            xorg.libXft
          ];

          makeFlags = [ "CC=${stdenv.cc.targetPrefix}cc" ];

          prePatch = ''
            sed -i "s@/usr/local@$out@" config.mk
            cp ${configFile} config.def.h
          '';

          meta = {
            mainProgram = "dwm";
          };
        };
    };
}
