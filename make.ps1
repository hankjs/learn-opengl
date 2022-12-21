param(
    [Parameter()]
    [switch] $Clean = $false,
    [switch] $Configure = $false,
    [switch] $Launch = $false,
    [switch] $Build = $false,
    [switch] $Sandbox = $false,
    [switch] $Glad = $false,
    [switch] $GLFW = $false
)

function Run {
    param (
        [Parameter()]
        [string] $Cmd,
        [string[]] $ArgumentList
    )
    
    $output = $Cmd 
    $output += " "
    $output += $ArgumentList 
    Write-Output $output
    Start-Process $Cmd -Wait -ArgumentList $ArgumentList -NoNewWindow
}

function Launch {
    Run "$PWD\bin\Debug-windows-x86_64\Sandbox\Sandbox.exe"
}

function Configure {
    Run "$PWD\vendor\bin\premake\premake5" 'vs2022'
}

function Clean {
    Remove-Item -Recurse -Force .\bin
    Remove-Item -Recurse -Force .\bin-int
}

$ProjectSln = "LearnOpenGL.sln"
function Glad {
    Run "devenv" $ProjectSln, "/Build", 'Debug', "/Project", "$PWD\vendor\Glad\Glad.vcxproj", "/Projectconfig", "Debug"
}
function GLFW {
    Run "devenv" $ProjectSln, "/Build", 'Debug', "/Project", "$PWD\vendor\GLFW\GLFW.vcxproj", "/Projectconfig", "Debug"
}
function Sandbox {
    Run "devenv" $ProjectSln, "/Build", 'Debug', "/Project", "$PWD\Sandbox\Sandbox.vcxproj", "/Projectconfig", "Debug"
}

if ($Clean.IsPresent) {
    Clean
}

if ($Configure.IsPresent) {
    Configure
}

if ($Build.IsPresent) {
    Glad
    GLFW
    Sandbox
} else {
    if ($Glad.IsPresent) {
        Glad
    }
    if ($GLFW.IsPresent) {
        GLFW
    }
    if ($Sandbox.IsPresent) {
        Sandbox
    }
}

if ($Launch.IsPresent) {
    Launch
}
