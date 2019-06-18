/*!
 * FileInput Brazillian Portuguese Translations
 *
 * This file must be loaded after 'fileinput.js'. Patterns in braces '{}', or
 * any HTML markup tags in the messages must not be converted or translated.
 *
 * @see http://github.com/kartik-v/bootstrap-fileinput
 *
 * NOTE: this file must be saved in UTF-8 encoding.
 */
(function ($) {
    "use strict";

    $.fn.fileinputLocales['pt-BR'] = {
        fileSingle: 'arquivo',
        filePlural: 'arquivos',
        browseLabel: 'Procurar&hellip;',
        removeLabel: 'Remover',
        removeTitle: 'Remover arquivos selecionados',
        cancelLabel: 'Cancelar',
        cancelTitle: 'Interromper envio em andamento',
        uploadLabel: 'Enviar',
        uploadTitle: 'Enviar arquivos selecionados',
        msgSizeTooLarge: 'O arquivo "{name}" (<b>{size} KB</b>) excede o tamanho mÃ¡ximo permitido de <b>{maxSize} KB</b>. Por favor, tente enviar novamente!',
        msgFilesTooLess: 'VocÃª deve selecionar pelo menos <b>{n}</b> {files} para enviar. Por favor, tente enviar novamente!',
        msgFilesTooMany: 'O nÃºmero de arquivos selecionados para o envio <b>({n})</b> excede o limite mÃ¡ximo permitido de <b>{m}</b>. Por favor, tente enviar novamente!',
        msgFileNotFound: 'O arquivo "{name}" nÃ£o foi encontrado!',
        msgFileSecured: 'RestriÃ§Ãµes de seguranÃ§a impedem a leitura do arquivo "{name}".',
        msgFileNotReadable: 'O arquivo "{name}" nÃ£o pode ser lido.',
        msgFilePreviewAborted: 'A prÃ©-visualizaÃ§Ã£o do arquivo "{name}" foi interrompida.',
        msgFilePreviewError: 'Ocorreu um erro ao ler o arquivo "{name}".',
        msgInvalidFileType: 'Tipo invÃ¡lido para o arquivo "{name}". Apenas arquivos "{types}" sÃ£o permitidos.',
        msgInvalidFileExtension: 'ExtensÃ£o invÃ¡lida para o arquivo "{name}". Apenas arquivos "{extensions}" sÃ£o permitidos.',
        msgValidationError: 'Erro de envio de arquivo',
        msgLoading: 'Enviando arquivo {index} de {files}&hellip;',
        msgProgress: 'Enviando arquivo {index} de {files} - {name} - {percent}% completo.',
        msgSelected: '{n} {files} selecionado(s)',
        msgFoldersNotAllowed: 'Arraste e solte apenas arquivos! {n} soltar pasta(s) ignoradas.',
        dropZoneTitle: 'Arraste e solte os arquivos aqui&hellip;'
    };
})(window.jQuery);