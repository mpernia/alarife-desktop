/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

CKEDITOR.editorConfig = function( config )
{
	// Define changes to default configuration here. For example:
	// config.language = 'fr';
	// config.uiColor = '#AADC6E';


config.language = 'es';
	
config.toolbar_Full =
[
    { name: 'document',    items : [ 'Source','-','Preview','Print','-','Templates' ] },
    { name: 'clipboard',   items : [ 'Cut','Copy','Paste','PasteText','PasteFromWord','-','Undo','Redo' ] },
    { name: 'editing',     items : [ 'Find','Replace','-','SelectAll' ] },
    { name: 'insert',      items : [ 'Table','HorizontalRule','SpecialChar','PageBreak' ] },
    { name: 'links',       items : [ 'Link','Unlink','Anchor' ] },
    { name: 'colors',      items : [ 'TextColor','BGColor' ] },
    { name: 'tools',       items : [ 'ShowBlocks' ] },
    { name: 'basicstyles', items : [ 'Bold','Italic','Underline','Strike','Subscript','Superscript','-','RemoveFormat' ] },
    { name: 'paragraph',   items : [ 'NumberedList','BulletedList','-','Outdent','Indent','-','Blockquote','CreateDiv','-','JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock','-','BidiLtr','BidiRtl' ] },
    { name: 'styles',      items : [ 'Styles','Format','Font','FontSize' ] }
];

config.extraPlugins += 'tableresize,';
config.extraPlugins += 'tabletools,';
config.extraPlugins += 'docprops,';
config.removePlugins += 'resize,';
config.fullPage = true;
config.height = 500;

};
