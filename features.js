var releaseFeatures = [
    {
        name:"v0.0-alpha",
        features: [
            "Create new images",
            "Load existing images",
            "Transfer image from canvas to edit area",
            "Transfer image from edit area to canvas",
			"Pencil Tool",
			"Color Pick",
			"Save image (png, jpg, bmp)",
			"32 color bit image format only",
			"Language Options (Portuguese and English only for now)",
			"Set tile size",
			"Toggle Transparency from pick and paste mode.",
			"Save app state through multiple executions.",
			"Basic Static Color Palette"
        ]
    },
    {
        name:"v0.01-alpha",
        features:[
			"Line Tool",
			"Ellipse Tool",
			"Rectangle Tool",
			"Flood Fill (Bucket) Tool"
        ]
    },
    {
        name:"v0.02-alpha (current release)",
        features:[
			"Existing Tools Bugfixes",
			"Select new image color on dialog",
			"Limited Undo/Redo",
			"Support for other image extensions <br>  (*.png *.bmp *.jpg *.jpeg *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"
        ]
    },
	{
        name:"Implemented Features",
        features:[
			"Improved Undo/Redo",
			"Automatic Color Gradient",
			"Swap Colors",
			"Bug Fixes"
        ]
    },
	{
        name:"Planned Features",
        features:[
			"Improved Color Palette Options (Save/Load)",
			"Text Tool",
			"Zoom",
			"Selection Tool",
			"Copy",
			"Paste",
			"Show grid",
			"Show Divisions",
			"Change Image Size",
			"Resize",
			"Shift Selection",
			"Different image formats (8 bit indexed and 32 bit)",
			"Palette for different Modes (Indexed, 32 bit)",
			"Animate Sprites"
        ]
    },
	{
        name:"Possible Features",
        features:[
			"Animation Export (.gif)",
			"Voxel Image Editor",
			"Project File (Program specific settings)",
			"Multi Layer edit mode"
        ]
    }
];

function LoadFeatures(element){
    element.innerHTML = "";
    for(var i=0;i<releaseFeatures.length;i++){
        element.innerHTML += "<h4>"+releaseFeatures[i].name+"</h4>\n";
		element.innerHTML += "<ul>\n";
        for(var j=1;j<releaseFeatures[i].features.length;j++){
            element.innerHTML += "<li>"+releaseFeatures[i].features[j]+"</li>\n";
        }
		element.innerHTML += "</ul>\n";
    }
}