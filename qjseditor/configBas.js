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
    { name: 'clipboard',   items : [ 'Cut','Copy','Paste','PasteText','PasteFromWord','-','Undo','Redo' ] },
    { name: 'editing',     items : [ 'Find','Replace','-','SelectAll' ] },
    { name: 'insert',      items : [ 'SpecialChar' ] },
    { name: 'links',       items : [ '' ] },
    { name: 'colors',      items : [ 'TextColor','BGColor' ] },
    { name: 'tools',       items : [ 'ShowBlocks' ] },
    { name: 'basicstyles', items : [ 'Bold','Italic','Underline','Strike','Subscript','Superscript','-','RemoveFormat' ] },
    { name: 'paragraph',   items : [ 'NumberedList','BulletedList','-','Outdent','Indent','-','JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock' ] },
	{ name: 'styles',      items : [ 'Format','Font','FontSize' ] }
];

config.extraPlugins += 'tableresize,';
config.extraPlugins += 'tabletools,';
config.extraPlugins += 'docprops,';
config.removePlugins += 'resize,';
config.fullPage = true;
config.height = 300;

};
