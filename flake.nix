{
  description = "dwm-6.5";

  inputs.nixpkgs.url = "nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs }:
    let
      configFile = import ./config.nix { inherit nixpkgs; };
    in
    {
      defaultPackage.x86_64-linux =
        with import nixpkgs { system = "x86_64-linux"; };
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
