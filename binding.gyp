{
    "targets": [
        {
            "target_name": "fontinfo",
            "sources": [
                "src/FontOperations.cc",
                "src/FontInfo.cc"
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
            "link_settings": {
                "libraries": ["DWrite.lib"]
            }
        }
    ]
}