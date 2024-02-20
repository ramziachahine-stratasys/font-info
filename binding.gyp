{
    "targets": [
        {
            "target_name": "font-info",
            "sources": ["src/FontInfo.cc"],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
            "link_settings": {
                "libraries": ["Dwrite.lib"]
            }
        }
    ]
}