
<form class="form-horizontal">
    <fieldset>
<br>
<br>
     <!--  Form Name
        <legend>Municípios Território</legend>-->
        <div class="form-group">
            <label class="col-md-2 "></label>
            <div class="col-md-4">
                <p style="color: red">Caso o Município desejado não seja exibido na caixa de seleção, verifique se ele ja não se encontra cadastrado na listagem abaixo.</p>
            </div>
        </div>
        <!-- Select Basic -->
        <div class="form-group">
            <label class="col-md-2 control-label " for="estauf">UF: </label>
            <div class="col-md-4">
                <select name="estuf" id="estuf" class="form-control chosen"
                        onchange="javascript:filtraMunicipios(this.value);">
                    <option value="0" >Selecione</option>
                    <?php
                    $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                    $estados = $db->carregar($sql);
                    foreach ($estados as $chave => $estado) { ?>
                        <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                    <?php } ?>
                </select>

            </div>
        </div>

        <!-- Select Basic -->
        <div class="form-group" >
            <label class="col-md-2 control-label" for="muncod">Município: </label>
            <div class="col-md-4" id="divFiltroMunicipio">
                <select id="muncod[]" name="muncod" class="form-control">
                    <option value="">Selecione </option>
                </select>
            </div>
        </div>

        <!-- Button -->
        <div class="form-group">
            <label class="col-md-2 "></label>
            <div class="col-md-4">
                <button id="btnAddMunicipio" name="btnAddMunicipio"  type="button" class="btn btn-primary btn-block"><span class="glyphicon glyphicon-plus"></span> Adicionar</button>
            </div>
        </div>
    </fieldset>
</form>

<div class="container" id="divmunicipiocadastrados">

    <table class="table table-hover" >
        <thead style="text-align: left">
        <tr style="text-align: left">
            <th style="text-align: left">Ação</th>
            <th style="text-align: left">UF</th>
            <th style="text-align: left">Município</th>
        </tr>
        </thead>
        <tbody>
        <?php
        if(!empty($arrTerritorioMunicipio)){

            foreach ($arrTerritorioMunicipio as $territorioMunicipio):?>


                <tr>
                    <td><button class="btn btn-danger" type="button" onclick="deletamunicipio(<?=$territorioMunicipio['termunid']?>)" ><span class="glyphicon glyphicon-trash"></span></button></td>
                    <td><?=$territorioMunicipio['estuf']?></td>
                    <td><?=$territorioMunicipio['mundescricao']?></td>
                </tr>
            <?php endforeach;
        }else{
            ?>
            <tr>
                <td  colspan="3">
                    <div class="alert alert-info center" style="text-align: center">
                        Nenhum dado Encontrado!
                    </div>
                </td>

            </tr>

            <?php
        }
        ?>
        </tbody>
    </table>
</div>