#!/bin/bash

TELEGRAM_BOT_TOKEN="6905647145:AAFQq3UCPJDHAGX2nXtYoZOqgAI0Bnra3_g"

TELEGRAM_USER_ID="530284066"
NL="%0A" # new line
Quotes="%22"
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
EMOJI_GREEN_CIRCLE="%F0%9F%9F%A2"
EMOJI_RED_CIRCLE="%F0%9F%94%B4"
EMOJI_NEXT_TRACK="%23%ED%FE%0F"
EMOJI_YELLOW_CIRCLE="%F0%9F%9F%A1"

TEXT=$EMOJI_RED_CIRCLE
if [ "$(echo "$CI_JOB_STATUS" | grep -c "success")" -gt 0 ]; then
    TEXT=$EMOJI_GREEN_CIRCLE
fi
TEXT="${TEXT} Project:+<a+href=$Quotes$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/$Quotes>$CI_PROJECT_NAME</a>$NL"
TEXT="${TEXT}Branch:+$CI_COMMIT_REF_SLUG$NL"
TEXT="${TEXT}CI/CD+job+stage:+<b>$CI_JOB_STAGE</b>$NL"
TEXT="${TEXT}Status:+<b>$CI_JOB_STATUS</b>.$NL"
if [ $# != 0 ]; then 
    TEXT="${TEXT}${NL}"
    NEXT_SCRIPT_URL=""
    if [ "$(echo "$1" | grep -c "manual")" -gt 0 ]; then
        TEXT="${TEXT}${EMOJI_YELLOW_CIRCLE} "
        NEXT_SCRIPT_URL="$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/"
    fi
    TEXT="${TEXT}Next+expected+stage: <a href=\"$NEXT_SCRIPT_URL\">$1</a>"
else
    TEXT="${TEXT}${NL}Pipeline completed successfully!"
fi

curl -s -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT&parse_mode=HTML" $URL > /dev/null