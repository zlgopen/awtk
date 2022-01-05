/**
 * 这个文件单纯用于脚本生成第三方库的导出函数
 *
 */

/**
 * @method SDL_ShowCursor
 * Toggle whether or not the cursor is shown.
 * @annotation ["global"]
 * @param {int} toggle    1 to show the cursor, 0 to hide it, -1 to query the current state.
 * @return {int}    1 if the cursor is shown, or 0 if the cursor is hidden.
 */

/**
 * @method SDL_WarpMouseInWindow
 * Moves the mouse to the given position within the window.
 * @annotation ["global"]
 * @param {SDL_Window*} window    1 to show the cursor, 0 to hide it, -1 to query the current state.
 * @param {int} x    The x coordinate within the window.
 * @param {int} y    The y coordinate within the window.
 */

/**
 * @method SDL_GetDefaultCursor
 * Return the default cursor.
 * @annotation ["global"]
 * @return {SDL_Cursor*}    the default cursor.
 */

/**
 * @method SDL_SetCursor
 * Set the active cursor.
 * @annotation ["global"]
 * @param {SDL_Cursor*} cursor    cursor.
 */

/**
 * @method SDL_FreeCursor
 * Frees a cursor created with SDL_CreateCursor() or similar functions.
 * @annotation ["global"]
 * @param {SDL_Cursor*} cursor    cursor.
 */

/**
 * @method SDL_CreateSystemCursor
 * Create a system cursor.
 * @annotation ["global"]
 * @param {SDL_SystemCursor} id    id.
 * @return {SDL_Cursor*}    the cursor.
 */

/**
 * @method SDL_GetMouseFocus
 * Get the window which currently has mouse focus.
 * @annotation ["global"]
 * @return {SDL_Window*}    the window.
 */

/**
 * @method SDL_MixAudioFormat
 * @annotation ["global"]
 * @param {Uint8*} dst
 * @param {const Uint8*} src
 * @param {SDL_AudioFormat} format
 * @param {Uint32} len
 * @param {int} volume
 */

/**
 * @method SDL_DequeueAudio
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev    The device ID from which we will dequeue audio.
 * @param {void*} data    A pointer into where audio data should be copied.
 * @param {Uint32} len    The number of bytes (not samples!) to which (data) points.
 * @return {Uint32}    number of bytes dequeued, which could be less than requested.
 */

/**
 * @method SDL_QueueAudio
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev    The device ID to which we will queue audio.
 * @param {const void*} data    The data to queue to the device for later playback.
 * @param {Uint32} len    The number of bytes (not samples!) to which (data) points.
 * @return {Uint32}    0 on success, or -1 on error.
 */

/**
 * @method SDL_GetQueuedAudioSize
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev    The device ID of which we will query queued audio size.
 * @return {Uint32}    Number of bytes (not samples!) of queued audio.
 */

/**
 * @method SDL_ClearQueuedAudio
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev    The device ID of which to clear the audio queue.
 */

/**
 * @method SDL_PauseAudioDevice
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev
 * @param {int} pause_on
 */

/**
 * @method SDL_CloseAudioDevice
 * @annotation ["global"]
 * @param {SDL_AudioDeviceID} dev
 */

/**
 * @method SDL_OpenAudioDevice
 * @annotation ["global"]
 * @param {const char*} device
 * @param {int} iscapture
 * @param {const SDL_AudioSpec*} desired
 * @param {const SDL_AudioSpec*} obtained
 * @param {int} allowed_changes
 * @return {SDL_AudioDeviceID}    0 on error, a valid device ID that is >= 2 on success.
 */

/**
 * @method SDL_WasInit
 * @annotation ["global"]
 * @param {Uint32} flags
 * @return {Uint32}
 */

/**
 * @method SDL_Init
 * @annotation ["global"]
 * @param {Uint32} flags
 * @return {int}
 */

/**
 * @method SDL_GetError
 * @annotation ["global"]
 * @return {const char*}
 */
