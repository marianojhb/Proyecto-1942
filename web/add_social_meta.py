#!/usr/bin/env python3
"""Add social sharing metadata to Emscripten's generated HTML."""
from pathlib import Path
import sys

html = Path(sys.argv[1])
content = html.read_text(encoding="utf-8")
marker = "<!-- proyecto1942-social -->"
if marker not in content:
    tags = """<!-- proyecto1942-social -->
<meta name="description" content="Jugá Proyecto 1942 en tu navegador: un arcade desarrollado en C++ y adaptado a SDL2 y WebAssembly con Emscripten.">
<meta property="og:type" content="website">
<meta property="og:locale" content="es_AR">
<meta property="og:title" content="Proyecto 1942 | Arcade en el navegador">
<meta property="og:description" content="Jugá un arcade desarrollado en C++ y adaptado a SDL2 y WebAssembly con Emscripten.">
<meta property="og:url" content="https://proyecto-1942.marianojhb.is-a.dev/">
<meta property="og:image" content="https://proyecto-1942.marianojhb.is-a.dev/og-image.png">
<meta name="twitter:card" content="summary_large_image">
<meta name="twitter:title" content="Proyecto 1942 | Arcade en el navegador">
<meta name="twitter:description" content="Arcade en C++, SDL2 y WebAssembly, disponible en el navegador.">
<meta name="twitter:image" content="https://proyecto-1942.marianojhb.is-a.dev/og-image.png">
"""
    if "</head>" not in content:
        raise SystemExit("Missing </head> in generated Emscripten HTML")
    content = content.replace("</head>", tags + "</head>", 1)
    html.write_text(content, encoding="utf-8")
