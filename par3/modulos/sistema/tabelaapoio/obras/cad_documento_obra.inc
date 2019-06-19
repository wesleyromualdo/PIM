<?php
switch ($_REQUEST['req']) {
    default:
        break;
}
?>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Documentos de Obra - Cadastro</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Código: *</label>
                            <div class="col-sm-2">
                                <input type="text" name="inprgdsc" maxlength="8" class="form-control" />
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Descrição: *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Texto de Ajuda: *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-md-3 control-label">Situação: *</label>
                            <div class="col-md-7" style="text-align: left">
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="inprgatd_extraordinario" id="inprgatd_extraordinario1" value="t">
                                    <label for="inprgatd_extraordinario1"> Ativo </label>
                                </div>
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="inprgatd_extraordinario" id="inprgatd_extraordinario2" value="f">
                                    <label for="inprgatd_extraordinario2"> Inativo </label>
                                </div>
                            </div>
                        </div>
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Extensão: *</label>
                            <div class="col-sm-5 col-md-5 col-lg-5 ">
                                <div class="chosen-container chosen-container-single" style="width: 0px;" title="" id="nivel_chosen">
                                    <a class="chosen-single chosen-default" tabindex="-1"><span>Selecione</span><div><b></b></div></a>
                                    <div class="chosen-drop"><div class="chosen-search"><input type="text" autocomplete="off"></div><ul class="chosen-results"></ul></div>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Tamanho Mínimo (Kb): *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Tamanho Máximo (Kb): *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Quantidade Mínima: *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Quantidade Máxima: *</label>
                            <div class="col-sm-7">
                                <input type="text" name="inprgdsc" maxlength="255" class="form-control"/>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-default" id="btn-incluir" onclick="window.location.href='par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=documento_obra'">
                    <i class="fa fa-remove"></i> Cancelar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" >
                    <i class="fa fa-save"></i> Salvar
                </button>
            </div>
        </div>
    </div>
</div>