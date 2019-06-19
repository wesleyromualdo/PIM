<?php
set_time_limit(0);
$css = "<style>
            td{font-size: 12px;}
        </style>";

$html = $css; 
$total = count($dados);
$a=1;

foreach($dados as $key => $valor){            

    $html .= "<div style='width:1100px;'>";
    $html .="<table style='width:100%'>
            <tr>
              <td>{$valor['info']['estdescricao']}</td>
            <tr>
        </table>";        

    $html .="<table style='width:100%'  cellspacing='0' cellpadding='0'>
            <tr>
                <td rowspan='8' style='width: 40px; height: 200px; border:1px solid;' align='center'>
                    <img src='{$_SERVER['HTTP_ORIGIN']}/sase/images/infogerais.png' border='0'>
                </td>
                <td style='width:250px; border-top:1px solid; '>Plano estadual de educação</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['fmepme']}</td>
                <td rowspan='8' style='width:150px;border:1px solid; ' >{$valor['info']['fmeequipeatolegal']}</td>
            </tr>
            <tr>
                <td style='border-top:1px solid; '>Período de Avaliação previstos</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['periodo']}</td>
            </tr>
            <tr>
                <td style='border-top:1px solid; '>Primeiro Ano de Avaliação</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['fmeperavalano1']}</td>
            </tr>
            <tr>
                <td style='border-top:1px solid; '>Comissão Coordenadora</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['fmecomcoordenadora']}</td>
            </tr>
            <tr>
                <td style='border-top:1px solid;'>Equipe Técnica</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['fmeequipetecnica']}</td>
            </tr>
            <tr>
                <td colspan='2' align='center' style='border-top:1px solid; '>Contatos</td>
            </tr>
            <tr>
                <td style='border-top:1px solid; '>Telefone</td>
                <td style='border-top:1px solid; border-left:1px solid;'>{$valor['info']['fmeequipetelefone']}</td>
            </tr>
            <tr>
                <td style='border-top:1px solid; border-bottom:1px solid;'>Email</td>
                <td style='border-top:1px solid; border-left:1px solid; border-bottom:1px solid;'>{$valor['info']['fmeequipeemail']}</td>
            </tr>
        </table>";

        $mtotal = 1;

        if($valor['meta']){
            foreach($valor['meta'] as $id => $vo){
                if($vo['dsc']){
                    $ultimo = $id;
                }
            }            

            foreach($valor['meta'] as $id => $vr){
                if($vr['dsc']){

                    $html .="<div style='border:1px solid; padding: 5px; font-size: 12px; margin-top: 25px; margin-bottom: 5px;'>
                                <b>Meta {$mtotal}:</b> {$vr['dsc']['fbmnumero']}.{$vr['dsc']['fbmdescricao']}
                            </div>";

                    $html .="<table style='width:100%' cellspacing='0' cellpadding='0'>
                        <tr>
                            <td style='width: 40px; height: 200px;  border:1px solid;' align='center'>
                                <img src='{$_SERVER['HTTP_ORIGIN']}/sase/images/estrategia.png' border='0'>
                            </td>
                            <td valign='top' style='border-top:1px solid; border-bottom:1px solid; border-right:1px solid;'>";

                                if($vr["estrategia"]){

                                    $html .="<table style='width:100%'  cellspacing='0' cellpadding='0'>
                                                <tr>
                                                    <td align='center' style='border-bottom:1px solid; border-right:1px solid;background-color: #93a4c1;'>Estratégia</td>
                                                    <td align='center' style='border-bottom:1px solid; border-right:1px solid;background-color: #93a4c1;'>Prazo</td>
                                                    <td align='center' style='border-bottom:1px solid; border-right:1px solid;background-color: #93a4c1;'>Previsões Orçamentárias</td>
                                                    <td align='center' style='width:150px; border-bottom:1px solid;background-color: #93a4c1;'>Alcançou Estratégia</td>
                                                </tr>";


                                        foreach($vr["estrategia"] as $vlr){        
                                            $html .="<tr>
                                                        <td style='border-bottom:1px solid; border-right:1px solid;'>{$vlr['fbmnumero']}.{$vlr['fbenumero']} {$vlr['fbedescricao']} </td>
                                                        <td style='border-bottom:1px solid; border-right:1px solid;' align='center'>{$vlr['fbeprazo']}</td>
                                                        <td style='border-bottom:1px solid; border-right:1px solid;'>{$vlr['fbeprevisao']}</td>
                                                        <td  align='center' style='border-bottom:1px solid;'>{$vlr['fbealcancou']}</td>
                                                    </tr>";
                                        }        

                                            $html .="</table>";	  
                                }else{
                                    $html .="<div style='padding:5px'>Não adicionado.</div>";
                                }

                        $html .= "</td>
                        </tr>
                    </table>

                    <br/>

                    <table style='width:100%' cellspacing='0' cellpadding='0'>
                        <tr>
                            <td style='width: 40px; height: 200px;  border:1px solid;' align='center'>
                                <img src='{$_SERVER['HTTP_ORIGIN']}/sase/images/monitoramento.png' border='0'>
                            </td>
                            <td valign='top' style='border-top:1px solid; border-bottom:1px solid; border-right:1px solid;'>";
                            if($vr["indicador"]){
                                $tind = 1;
                                foreach($vr["indicador"] as $fbsid => $vl){
                                    $border = ($tind < count($vr["indicador"]) || count($vr["indicador"]) == 1)?'border-bottom:1px solid;':'';
                                    $html .="<table style='width:100%;'  cellspacing='0' cellpadding='0'>
                                        <tr>
                                            <td colspan='12' style='border-bottom:1px solid; border-right:1px solid; font-weight: bold;'>{$vl["dados"]['fbmnumero']}.{$vl["dados"]['fbsnumero']} {$vl["dados"]['fbsdescricao']}</td>
                                            <td colspan='6'  style='border-bottom:1px solid;'><b>Prazo:</b> {$vl["dados"]['fbsprazo']}, <b>Alcançou indicador:</b>  {$vl["dados"]['fbsaalcancou']}</td>
                                        </tr>
                                        <tr>
                                          <td rowspan='2' style='border-bottom:1px solid; border-right:1px solid;'>Meta Prevista</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2014</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2015</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2016</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2017</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2018</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2019</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2020</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2021</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2022</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2023</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2024</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2025</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2026</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2027</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2028</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2029</td>
                                          <td style='border-bottom:1px solid; background-color: #93a4c1;' align='center'>2030</td>
                                        </tr>
                                        <tr>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2014]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2015]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2016]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2017]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2018]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2019]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2020]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2021]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2022]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2023]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2024]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2025]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2026]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2027]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2028]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2029]['fcmmetaprevista']}</td>
                                          <td style='border-bottom:1px solid; width: 40px;' align='center'>{$vl["ano"][2030]['fcmmetaprevista']}</td>
                                        </tr>
                                        <tr>
                                          <td rowspan='2' style='border-bottom:1px solid; border-right:1px solid;'>Meta Executada no Período <br> Dado Oficial</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2014</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2015</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2016</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2017</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2018</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2019</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2020</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2021</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2022</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2023</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2024</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2025</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2026</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2027</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2028</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2029</td>
                                          <td style='border-bottom:1px solid; background-color: #93a4c1;' align='center'>2030</td>
                                        </tr>
                                        <tr>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2014]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2015]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2016]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2017]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2018]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2019]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2020]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2021]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2022]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2023]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2024]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2025]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2026]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2027]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2028]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2029]['fcmmetaexecutadaoficial']}</td>
                                          <td style='border-bottom:1px solid; width: 40px;' align='center'>{$vl["ano"][2030]['fcmmetaexecutadaoficial']}</td>
                                        </tr>
                                        <tr>
                                          <td rowspan='2' style='{$border}border-right:1px solid;'>Meta Executada no Período <br> Dado Estadual</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2014</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2015</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2016</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2017</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2018</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2019</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2020</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2021</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2022</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2023</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2024</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2025</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2026</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2027</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2028</td>
                                          <td style='border-bottom:1px solid; border-right:1px solid; background-color: #93a4c1;' align='center'>2029</td>
                                          <td style='border-bottom:1px solid; background-color: #93a4c1;' align='center'>2030</td>
                                        </tr>

                                        <tr>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2014]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2015]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2016]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2017]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2018]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2019]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2020]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2021]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2022]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2023]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2024]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2025]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2026]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2027]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2028]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} border-right:1px solid; font-weight: bold; width: 40px;' align='center'>{$vl["ano"][2029]['fcmmetaexecutadaalternativa']}</td>
                                          <td style='{$border} width: 40px;' align='center'>{$vl["ano"][2030]['fcmmetaexecutadaalternativa']}</td>
                                        </tr>
                                    </table>";

                                    $tind++;
                                }



                            }else{
                                $html .="<div style='padding:5px'>Não adicionado.</div>";
                            }    
                        $html .="</td>
                        </tr>
                    </table>";

                   if($id != $ultimo) $html .= "<page_break>";
                    $mtotal++;
                }
            }
        }else{
            $html .='<div style="border:1px solid; padding: 5px; font-size: 12px; margin-top: 25px; margin-bottom: 5px;">
                        Meta não adicionada
                    </div>';

            $html .='<table style="width:100%" cellspacing="0" cellpadding="0">
                <tbody><tr>
                    <td style="width: 40px; height: 200px;  border:1px solid;" align="center">
                        <img src="'.$_SERVER['HTTP_ORIGIN'].'/sase/images/estrategia.png" border="0">
                    </td>
                    <td valign="top" style="border-top:1px solid; border-bottom:1px solid; border-right:1px solid;"><div style="padding:5px">Não adicionado.</div></td>
                </tr>
            </tbody></table>';                        

            $html .='<br>';

            $html .='<table style="width:100%" cellspacing="0" cellpadding="0">
                <tbody><tr>
                    <td style="width: 40px; height: 200px;  border:1px solid;" align="center">
                        <img src="'.$_SERVER['HTTP_ORIGIN'].'/sase/images/monitoramento.png" border="0">
                    </td>
                    <td valign="top" style="border-top:1px solid; border-bottom:1px solid; border-right:1px solid;"><div style="padding:5px">Não adicionado.</div></td>
                </tr>
            </tbody></table>'; 
        }    

        $html .="</div>";
        if($a < $total) $html .= "<page_break>";
        $a++;
}
        
if($_REQUEST['requisicao'] == 'xls'){
    header("Content-type: application/force-download");
    header ("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header ("Cache-Control: no-cache, must-revalidate");
    header ("Pragma: no-cache");
    header ("Content-type: application/x-msexcel");
    header ("Content-Disposition: attachment; filename=\"fichatecnica.xls\"" );
    header ("Content-Description: PHP Generated Data" );
    
    echo $html;
}

if($_REQUEST['requisicao'] == 'pdf'){
    
    include  APPRAIZ.'includes/mpdf/mpdf.php';

     $mpdf = new mPDF('utf-8', 'A4-L',0,'',3,3,23,16,3,3);

    $mpdf->SetHeader((htmlCabecalhoMpdf()));
    $mpdf->setFooter('<div style="border-bottom: 1px solid #ccc !important; margin-top:10px;"></div>{PAGENO}/{nbpg}');    
    
    $mpdf->WriteHTML(($html));
    $mpdf->Output('fichatecnica.pdf','D');    
}

function htmlCabecalhoMpdf()
{
    $usunome    =  $_SESSION['usunome'];
    $usucpf     =  $_SESSION['usucpf'];
    $usuorgao   =  (boolean)$_SESSION['usuorgao']?$_SESSION['usuorgao']:'Não vinculado';
    $data       = date("d/m/Y - H:i:s");

    $html = '<table width="100%" border="0" cellpadding="0" cellspacing="0">
                <tr bgcolor="#ffffff"> 
                    <td><img src="http://simec.mec.gov.br/imagens/brasao.gif" width="50" height="50" border="0"></td>
                    <td style="font: 9pt Arial,verdana;">
                        SIMEC<br>
                        Ministério da Educação / SASE - Secretaria de Articulação com os Sistemas de Ensino <br>
                        DTI - Diretoria de Tecnologia da Informação<br>	
                    </td>
                    <td align="right"  style="font: 9pt Arial,verdana;">
                        Impresso por: <strong>'.$usunome.'</strong><br>
                        Hora da Impressão: <strong>'.$data.'</strong>
                    </td>
                </tr>
            </table><div style="border-bottom: 1px solid #ccc !important; margin-top:10px;"></div>';

    return $html;
}
?>