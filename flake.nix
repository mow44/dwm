{
  description = "dwm-6.5";

  inputs.nixpkgs.url = "nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      configFile = import ./config.nix { inherit pkgs; };
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
