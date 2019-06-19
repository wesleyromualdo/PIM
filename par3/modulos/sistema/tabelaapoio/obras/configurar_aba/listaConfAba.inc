<table class="table">
    <thead>
        <tr>
            <th>Visível</th>
            <th>Aba</th>
            <th>Configurar</th>
        </tr>
    </thead>
    <tbody>
    <?php foreach($arrAbas as $aba){?>
        <tr>
            <td>
            <?php if($aba['oabid'] > '2'){?>
                <input type="checkbox" class="oabid"
                       oabid="<?php echo $aba['oabid']?>" ocaid="<?php echo $aba['ocaid']?>" conf="<?php echo in_array($aba['oabid'], $arrAbaConf); ?>"
                       <?php echo $aba['ocaid'] != '' ? 'checked="checked"' : ''; ?>/>
            <?php }?>
            </td>
            <td><?php echo $aba['oabdsc']?></td>
            <td>
                <?php if(in_array($aba['oabid'], $arrAbaConf)){ ?>
                <span style="cursor:pointer;display:<?php echo $aba['ocaid'] != '' ? '' : 'none'; ?>" class="configurar" id="conf_<?php echo $aba['oabid']?>" ocaid="<?php echo $aba['ocaid']?>" data-toggle="modal" data-target="#modal">
                    <i class="fa fa-search" aria-hidden="true"></i>
                </span>
                <?php } ?>
            </td>
        </tr>
    <?php }?>
    </tbody>
</table>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog" id="formContent">
    </div>
</div>
<script>

var otpid = '<?php echo $arrPost['otpid']?>';

    $(document).on('change','.oabid',function () {
        var oabid   = $(this).attr('oabid');
        var ocaid   = $(this).attr('ocaid');
        var conf    = $(this).attr('conf');

        var status  = '';
        if($(this).is(':checked')){ status = 'A'; }else{ status = 'I'; }

        if(conf == '1'){
            if(status == 'A'){
                $('#conf_'+oabid).show();
            }else{
                $('#conf_'+oabid).hide();
            }
        }
    	var caminho = window.location.href;
        var action  = '&requisicao=vinculaAba&oabid=' + oabid + '&ocaid=' + ocaid + '&otpid=' + otpid + '&ocastatus=' + status;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#conf_'+oabid).attr('ocaid', resp);
            }
        });
    }).change();

    $(document).on('click','.configurar',function () {
        var ocaid   = $(this).attr('ocaid');
    	var caminho = window.location.href;
        var action  = '&requisicao=formConfiguracao&ocaid=' + ocaid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#formContent').html(resp);
            }
        });
    }).change();



</script>