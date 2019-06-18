<style>
    @media print {
        .ptable{
            width: 100%;
            padding-bottom: 20px;
        }
        .ptable-false{
            width: 100%;
            padding-bottom: 20px;
        }
        .ptable-bordered{
            border:1px solid;
            border-collapse: collapse;
        }

        .ptable-bordered-false{
            border: none;
        }

        .ptable-bordered tr td{
            border:1px solid;
            paddin: 5px;
        }
        .ptable tr td{
            border:1px solid;
            paddin: 5px;
        }
        .ptable .ptable-bordered th{
            border:1px solid;
            padding: 5px;
        }

        .pth {
            border:1px solid;
            paddin: 5px;
            text-align: center;
            background-color:#C4C6C9;
            padding: 10px;

        }

        .pcol-md-12{
            width: 100%;
        }
        .pcol-md-2{
            width: 10%;
        }

        .pcol-md-4{
            width: 40%;
        }

        .ptext-center{
            text-align: center;
            background-color:  #e6e6e6;
            padding: 5px;
            border:  1px solid;
        }
        .tdtext-center{
            text-align: center;
        }
        .classCabecalhoTd{
            padding: 5px;
        }
        .classTd{
            text-align: center;
        }
        .classCabecalhoTdFalse{
            border: none;
        }
    }

    .ptable{
        width: 100%;
        padding-bottom: 20px;
    }
    .ptable-false{
        width: 100%;
        padding-bottom: 20px;
    }
    .ptable-bordered{
        border:1px solid;
        border-collapse: collapse;
    }

    .ptable-bordered-false{
        border: none;
    }

    .ptable-bordered tr td{
        border:1px solid;
        paddin: 5px;
    }
    .ptable tr td{
        border:1px solid;
        paddin: 5px;
    }
    .ptable .ptable-bordered th{
        border:1px solid;
        padding: 5px;
    }

    .pth {
        border:1px solid;
        paddin: 5px;
        text-align: center;
        background-color:#C4C6C9;
        padding: 10px;

    }

    .pcol-md-12{
        width: 100%;
    }
    .pcol-md-2{
        width: 10%;
    }

    .pcol-md-4{
        width: 40%;
    }

    .ptext-center{
        text-align: center;
        background-color:  #e6e6e6;
        padding: 5px;
        border:  1px solid;
    }
    .tdtext-center{
        text-align: center;
    }
    .classCabecalhoTd{
        padding: 5px;
    }
    .classTd{
        text-align: center;
    }
    .classCabecalhoTdFalse{
        border: none;
    }

</style>
<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/8/2017
 * Time: 5:13 PM
 */?>
<div class="row">
    <div class="col-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th style='border: solid 1px #000000; padding:5px;'>Ano</th>
            <th  width="200px;" style='border: solid 1px #000000; padding:5px;'>Obra ID</th>
            <th  width="200px;" style='border: solid 1px #000000; padding:5px;'>Tipo de Obra</th>
            <th  width="200px;" style='border: solid 1px #000000; padding:5px;'>Nome da Obra</th>
            <th style='border: solid 1px #000000; padding:5px;'>Valor</th>
            <th style='border: solid 1px #000000; padding:5px;'>Situação</th>
            </thead>
            <tbody>
            <?php foreach($dadosIntemComposicaoObras as $key => $values):?>
                <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $values['obrano'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $values['obrid'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $values['obrid'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $values['octdsc'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'>R$ <?php echo number_format($values['obrvalor'],2,',','.');?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $values['esddsc'];?></td>
            </tr>
            <?php endforeach;?>
            </tbody>
        </table>
    </div>
</div>
<br/>