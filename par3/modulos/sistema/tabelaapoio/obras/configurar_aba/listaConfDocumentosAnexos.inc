<?php

$arrDocumentos = is_array($arrDocumentos) ? $arrDocumentos : array();

?>
<table class="table">
    <thead>
        <tr>
            <th width="15%"><center>Vinculados</center></th>
            <th width="65%">Tipo de Documento</th>
            <th width="20%">Ordem</th>
        </tr>
    </thead>
    <tbody>

    <?php foreach($arrDocumentos as $documento){?>
        <tr>
            <td>
                <center>
                    <input type="hidden" name="octid[<?php echo $documento['otdid']; ?>]" value="<?php echo $_REQUEST['octid']; ?>"/>
                    <input type="checkbox" name="otdid[<?php echo $documento['otdid']; ?>]" value="<?php echo $documento['otdid']; ?>" <?php echo $documento['octid'] != '' ? 'checked="checked"' : ''; ?> required="required"/>
                </center>
            </td>
            <td><?php echo $documento['otddsc']?></td>
            <td>
                <?php echo $simec->input('octordem['.$documento['otdid'].']', '', $documento['octordem'], array('maxlength'=>'2','type'=>'number','required'=>'required','id'=>'octordem_'.$documento['otdid']), array('input-size'=>10))?>
            </td>
        </tr>
    <?php }?>
    </tbody>
</table>
<div id="div-erro" style="display:none"><span class="danger">O campo não pode ser vazio</span></div>