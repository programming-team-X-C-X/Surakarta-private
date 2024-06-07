$logFile = "log.txt"
$dateTimeFormat = "yyyy-MM-dd HH:mm:ss"

# $list = 1..17
$list = @(
    4, 12)

$len = $list.Length

# Test result matrix
$res_matrix = @()
for ($i = 0; $i -lt $len; $i++) {
    $res_matrix += ,(@("") * $len)
}

for ($i = 0; $i -lt $list.Count; $i++) {
    for ($j = $i; $j -lt $list.Count; $j++) {
        $ri = $i
        $rj = $j
        for ($round = 0; $round -lt 2; $round++) {
            # Run twice for each pair
            # The ri is the server and rj is the client in the first round
            # Switch the role in the second round
            $serverProcess = Start-Process -FilePath "./Server_Team_$($list[$ri])/Server_Team_$($list[$ri]).exe" -ArgumentList "16677" -NoNewWindow
            Start-Sleep -Seconds 5

            if ($serverProcess.HasExited) {
                "$(Get-Date -Format $dateTimeFormat) Server process failed to start for Team $($list[$ri])." | Out-File $logFile -Append
                continue
            }

            $clientProcess1 = Start-Process -FilePath "./Client_Team_$($list[$ri])/Client_Team_$($list[$ri]).exe" -ArgumentList "127.0.0.1 16677 BLACK" -NoNewWindow
            $clientProcess2 = Start-Process -FilePath "./Client_Team_$($list[$rj])/Client_Team_$($list[$rj]).exe" -ArgumentList "127.0.0.1 16677 WHITE" -NoNewWindow
            Start-Sleep -Seconds 5

            if ($clientProcess1.HasExited) {
                "$(Get-Date -Format $dateTimeFormat) Client process failed to start for Team $($list[$ri])." | Out-File $logFile -Append
                continue
            }

            if ($clientProcess2.HasExited) {
                "$(Get-Date -Format $dateTimeFormat) Client process failed to start for Team $($list[$rj])." | Out-File $logFile -Append
                continue
            }

            Start-Sleep -Seconds 20

            $endTime = (Get-Date).AddMinutes(3)
            # Limit the execution time to 3 minutes

            $files = @(
                "./record/server/Team_$($list[$ri]).txt",
                "./record/client/Team_$($list[$ri]).txt",
                "./record/client/Team_$($list[$rj]).txt"
            )
            $found = $false

            while ((Get-Date) -lt $endTime -and -not $found) {
                foreach ($file in $files) {
                    if (Test-Path $file) {
                        try {
                            $content = Get-Content $file -Tail 2
                            if ($content -match '#') {
                                $found = $true
                                "$(Get-Date -Format $dateTimeFormat) Output completion detected in one of the files." | Out-File $logFile -Append
                                break
                            }
                        }
                        catch {
                            Write-Output "$(Get-Date -Format $dateTimeFormat) Error reading from $file. Error: $_" | Out-File $logFile -Append
                        }
                    }
                    else {
                        Write-Output "$(Get-Date -Format $dateTimeFormat) File not found yet: $file" | Out-File $logFile -Append
                    }
                }
                Start-Sleep -Seconds 10
            }

            if ($found) {
                $file1 = Get-Content $files[0] -Raw
                $file2 = Get-Content $files[1] -Raw
                $file3 = Get-Content $files[2] -Raw

                if ($file1 -eq $file2 -and $file2 -eq $file3) {
                    "$(Get-Date -Format $dateTimeFormat) All files are same." | Out-File $logFile -Append
                    $res_matrix[$ri][$rj] = "P"
                }
                else {
                    "$(Get-Date -Format $dateTimeFormat) Inconsistency detected!" | Out-File $logFile -Append
                    "$(Get-Date -Format $dateTimeFormat) Server: $($list[$ri])" | Out-File $logFile -Append
                    "$(Get-Date -Format $dateTimeFormat) Client: $($list[$ri])" | Out-File $logFile -Append
                    "$(Get-Date -Format $dateTimeFormat) Client: $($list[$rj])" | Out-File $logFile -Append
                    $res_matrix[$ri][$rj] = "F"
                }

            }
            else {
                "$(Get-Date -Format $dateTimeFormat) Timeout reached without detecting output completion." | Out-File $logFile -Append
                "$(Get-Date -Format $dateTimeFormat) Server: $($list[$ri])" | Out-File $logFile -Append
                "$(Get-Date -Format $dateTimeFormat) Client: $($list[$ri])" | Out-File $logFile -Append
                "$(Get-Date -Format $dateTimeFormat) Client: $($list[$rj])" | Out-File $logFile -Append
                $res_matrix[$ri][$rj] = "T"
            }

            # Terminate the processes by name
            $serverProcessName = "Server_Team_$($list[$ri])"
            $clientProcess1Name = "Client_Team_$($list[$ri])"
            $clientProcess2Name = "Client_Team_$($list[$rj])"

            Get-Process -Name $serverProcessName -ErrorAction SilentlyContinue | Stop-Process -Force
            Get-Process -Name $clientProcess1Name -ErrorAction SilentlyContinue | Stop-Process -Force
            Get-Process -Name $clientProcess2Name -ErrorAction SilentlyContinue | Stop-Process -Force

            $ri, $rj = $rj, $ri
        }

    }
}

$csvContent = @()

$header = "Server\Client" + "," + ($list -join ",")
$csvContent += $header

for ($i = 0; $i -lt $len; $i++) {
    $row = "Team_$($list[$i])" + "," + ($res_matrix[$i] -join ",")
    $csvContent += $row
}

# Output the result to a CSV file
$csvContent | Out-File -FilePath "results.csv" -Encoding utf8

"Results have been written to results.csv" | Out-File $logFile -Append

"" | Out-File $logFile -Append
