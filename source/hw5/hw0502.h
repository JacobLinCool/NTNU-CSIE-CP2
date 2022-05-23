/**
 * @brief ID3 Editor
 * See help for more information. "-h" for help.
 *
 * If there is no ID3 tag in the file, every action will add the tag to the target automatically if no error occurs.
 * The genre will be set to 255 (Unknown) by default.
 * If you don't want to keep the tag, use "-d" to delete it immediately.
 *
 * You can use multiple flags at once, the parsing order is:
 * `-t`, `-T`, `-a`, `-A`, `-y`, `-c`, `-g`, `-d`, `-l`
 */
