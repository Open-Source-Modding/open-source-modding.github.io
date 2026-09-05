#!/bin/bash
# Copy XeNTaX forum attachments to organized structure

SRC="/home/selene/Documents/Code/re/xentax/attachments/xentax"
DST="/home/selene/Documents/Code/game-tools/xentax"

# Function to copy if exists
copy_topic() {
    local forum=$1
    local topic=$2
    local game=$3
    local desc=$4
    
    if [ -d "$SRC/$forum/$topic" ]; then
        echo "Copying forum $forum topic $topic ($desc) -> $game"
        cp -r "$SRC/$forum/$topic" "$DST/$game/topic_${topic}_${desc}"
    else
        echo "NOT FOUND: forum $forum topic $topic ($desc)"
    fi
}

# Ghost Recon
# Topic 2712 - GRAW .bundle reader (forum 10)
copy_topic 10 2712 ghostrecon "graw_bundle_reader"

# Topic 3258 - L4D XZIP (forum 10) - but this is L4D not GR
# Actually let me check - 3258 is L4D 360.zip

# NFS topics in forum 10
copy_topic 10 1494 nfs "topic_1494"
copy_topic 10 1632 nfs "topic_1632"
copy_topic 10 3717 nfs "topic_3717"
copy_topic 10 3752 nfs "topic_3752"
copy_topic 10 4155 nfs "topic_4155"

# NFS topics in forum 18
copy_topic 18 4158 nfs "topic_4158"
copy_topic 18 4467 nfs "topic_4467"

# L4D topics
copy_topic 10 3258 left4dead "xzip_360"
copy_topic 18 3257 left4dead "vtfx_textures"

# Mirror's Edge Catalyst
copy_topic 17 14259 mirrors_edge "bf4dumper"

# Check for other known topics
# Ghost Recon: 1157, 4034, 4089, 9259, 9554, 9555
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 1157 4034 4089 9259 9554 9555; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic ghostrecon "topic_${topic}"
        fi
    done
done

# L4D: 3219, 7964
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 3219 7964; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic left4dead "topic_${topic}"
        fi
    done
done

# Sims 3: 3491, 5349, 5357, 5637, 5638, 9431
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 3491 5349 5357 5637 5638 9431; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic sims3 "topic_${topic}"
        fi
    done
done

# SimCity: 11840
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 11840; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic simcity "topic_${topic}"
        fi
    done
done

# Cities Skylines: 12681, 12761, 18672, 25039
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 12681 12761 18672 25039; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic citiesskylines "topic_${topic}"
        fi
    done
done

# Mirror's Edge original: 3245, 16367
for forum in 10 13 15 16 17 18 21 22 28 29 30 32 33 35 36 37 38 40; do
    for topic in 3245 16367; do
        if [ -d "$SRC/$forum/$topic" ]; then
            copy_topic $forum $topic mirrors_edge "topic_${topic}"
        fi
    done
done

# TDU / TDU2 / Test Drive series
copy_topic 10 2873 tdu "TDU1_formats"
copy_topic 10 5747 tdu "TDU2_BNK"
copy_topic 10 6021 tdu "TDU2_big"
copy_topic 10 6316 tdu "TDU2_Light_Launcher"
copy_topic 16 10680 tdu "TD5_car_models"
copy_topic 16 27002 tdu "TDU_3dg"
copy_topic 10 3281 tdu "TDU_EveOfDestruction"

echo "Done!"