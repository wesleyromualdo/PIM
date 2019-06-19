<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/9/2017
 * Time: 8:54 PM
 */?>
<style>
    .table{
        width: 100%;
    }
    .table-bordered{
        border:1px solid;
    }

    .col-md-12{
        width: 100%;
    }
    .col-md-2{
        width: 10%;
    }

    .col-md-4{
        width: 40%;
    }

</style>
<div>
    <div class="col-md-12">

        <table class="table table-bordered-false">
            <tr>
                <td><strong style="color:red;">Situação: Enviado para Análise</strong></td>
                <td><strong>Valor total planejado:</strong>
                    R$ <?php echo number_format($situacaoValorTotalPlanejado, 2, ',', '.'); ?></strong>
                </td>
            </tr>
        </table>
    </div>
</div>
