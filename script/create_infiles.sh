#!/bin/bash

dieasesFile=$1
countriesFile=$2
inputDir=$3
numFiles=$4
numRecords=$5

if [[ "$#" -ne 5 ]]; then
    echo "You must enter exactly 5 command line arguments"
    exit 1
fi

if [[ "$numFiles" < 1 ]];
then
    echo '>> Number of files provided is less than 1'
    exit 1
fi

if [[ "$numRecords" < 1 ]]
then
    echo '>> Number of records provided is less than 1'
    exit 1
fi

mkdir $inputDir

idCount=0

while read line; do
    mkdir $inputDir/$line
    cd $inputDir/$line
    for (( c=0; c<$numFiles; c++ ))
    do  
        day=$(($RANDOM%30+1))
        if [[ "$day" -lt 10 ]];
        then
            day="0${day}"
        fi

        month=$(($RANDOM%12+1))
        if [[ "$month" -lt 10 ]];
        then
            month="0${month}"
        fi
        year=$(($RANDOM%20+2001))
        newFile="${day}-${month}-$year"
        touch $newFile

        for (( i=0; i<$numRecords; i++ ))
        do  
            disease=`shuf -n 1 ../../$dieasesFile`
            choose=$(($RANDOM%2))
            status="ENTER"
            if [[ "$choose" == 0 ]];
            then 
                status="EXIT"
            fi
            chars=abcdefghijklmnopqrsrtvuwyz
            charsA=ABCDEFGHIJKLMNOPQRSRTVUWYZ
            firstName="${charsA:RANDOM%${#charsA}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}"
            lastName="${charsA:RANDOM%${#charsA}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}${chars:RANDOM%${#chars}:1}"
            age=$(($RANDOM%120+1))
            echo "$idCount $status $firstName $lastName $disease $age">> $newFile
            idCount=$(($idCount+1))
        done

    done
    cd ../..
done < $countriesFile
mv $inputDir ..