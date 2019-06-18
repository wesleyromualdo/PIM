<?php
global $simec;
?>
<form id='form-filtro-pendencias'>
    <div class="row">
        <div class="col-lg-offset-1 col-lg-11">
            <?php echo
            $radio .= $simec->checkbox(
                'tipid',
                null,
                [1,2,3,4,5,6,7,8],
                $this->pendencia,
                array(false, ''),
                array('type' => 'radio radio-info radio', 'style' => '')
            );
            ?>
            <button type='button' class='btn btn-success left' align="left" id='btn-filtro-pendencias'>Filtrar</button>
        </div>
    </div>

</form>


<script>
    $("#btn-filtro-pendencias").on('click',function () {
        var arTipid = [];$("input:checkbox[name=tipid]:checked").each(function(){
            arTipid.push($(this).val());
        });
        $.ajax({
            dataType: "html",
            url: '/par3/par3.php?modulo=painel/filtrarpendencia&acao=A',
            data: {
                tipid: arTipid
            },
            success: function(data) {
                $('#situacaoEntidades').html(data);
            }
        });
    });
</script>