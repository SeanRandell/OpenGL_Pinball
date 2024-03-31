param($SolutionDir)

# Define the folder containing the Shader files
$folderPath = "$SolutionDir\Src\Shaders"

# Get all files in the folder
$allFiles = Get-ChildItem -Path $folderPath

# Filter shader files based on their extensions
$shaderFiles = $allFiles | Where-Object { $_.Extension -match '\.vert$|\.frag$|\.geom$' }

# Define the C++ class name and output file path
$className = "ShaderSources"
$outputFilePath = "$SolutionDir\Src\ShaderSources.h"

# Start building the C++ class
$output = @"
#include <string>

class $className {
public:

"@

# Loop through each Shader file and append its content as a string in the C++ class
foreach ($file in $shaderFiles) {
    # Write-Host "Processing file: $($file.FullName)"
    $content = Get-Content -Path $file.FullName -Raw
    # Write-Host "Content for $($file.BaseName):"
    $content = $content -replace "`"", '\"'  # Escape double quotes
	$content = $content -replace "`r?`n", "`r`n" # Ensure all newlines end with CR LF
    $output += @"
    const char* $($file.BaseName) = R"(
${content}
)";


"@
}

# Close the class definition
$output += @"
};
"@

# Clean the existing content in the output file
Clear-Content -Path $outputFilePath

# Write the output to a file
$output | Out-File -FilePath $outputFilePath -Encoding utf8

Write-Host "C++ class file generated: $outputFilePath"
