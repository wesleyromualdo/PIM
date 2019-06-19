<?php
/**
 * View da representando a tabela obra_tipo
 *
 * @category visao
 * @version $Id$
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 */
if (IS_PRODUCAO) {
    $urlApiMaps = GMAPS_API.'libraries=places&region=br&language=pt-BR';
} else {
    $urlApiMaps = 'http://maps.google.com/maps/api/js?libraries=places&region=br&language=pt-BR&key=AIzaSyD8lBc5h54olh16VbV5rVzeRJWjLgGgd_o';
}
$obrid = (int)$_GET['obrid'];
$inuid = (int)$_GET['inuid'];
if (empty($inuid)) {
    echo 'Não possui inuid! ';
    exit;
}
$itrid = $controleUnidade->pegarItrid($inuid);
$controllerObra     = new Par3_Controller_Obra($inuid, $obrid);
$controleUnidade    = new Par3_Controller_InstrumentoUnidade();
$modelEstado        = new Par3_Model_Estado();
$ctrPlanejamento    = new Par3_Controller_IniciativaPlanejamento();

//$controleUnidade->pegarMuncod($inuid);
//$controleUnidade->pegarMunicipio($inuid);
//$controleUnidade->pegarItrid($inuid); // defini se e estadual ou municipal

$disabled = $_GET['disabled'] ? 'disabled' : '';
//$disabledEdicao = $controllerObra->verificarPerfilEdicao() ? '' : 'disabled';

$helperUrlToReturn = new Par3_Helper_UrlToReturnHelper();
//REGRA PARA ANÁLISE DE ENGENHARIA
$disableDadosBasicos = $_GET['disabled'];
$disableEndereco     = $_GET['disabled'];
$disableLatitude     = $_GET['disabled'];

//VALIDAÇÃO ANÁLISE DE ENGENHARIA
$mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$mObra = new Par3_Model_Obra($_GET['obrid']);
$disableDadosBasicos = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::DADOS_TERRENO,1,$_GET['obrid']);
$disableEndereco     = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::DADOS_TERRENO,2,$_GET['obrid']);
$disableLatitude     = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::DADOS_TERRENO,3,$_GET['obrid']);
//******************************************************
switch ($_POST['requisicao']) {
    case 'extrato':
        ob_clean();
        $controllerObra  = new Par3_Controller_Obra($_GET['inuid'],$_GET['obrid']);
        echo $controllerObra->gerarExtratoObra();
        die;
        break;
    case 'verificaCepMunicipio':
    case 'verificaCepUF':
        ob_clean();
        if(!$_POST['cep']) {
            die();
        }

        include_once APPRAIZ.'www/par/ajax.php';
        $_POST['requisicao']($_POST['cep']);
        die;
        break;
    case 'recuperaDescricaoMuncod':
        ob_clean();
        $mdMunicipio = new Territorios_Model_Municipio($_REQUEST['muncod']);
        echo $mdMunicipio->mundescricao;
        die();
        break;
    case 'recuperarmetragem':
        ob_clean();
        $ctrObraTipoVersaoProjeto   = new Par3_Controller_ObraTipoVersaoProjeto();
        $mVersaoProjeto = $ctrObraTipoVersaoProjeto->recuperar($_REQUEST['ovpid']);
        echo $mVersaoProjeto->ovpmetragem;die;
        break;
    case 'salvar':
        $controllerObra->salvar($_POST);
        break;
    case 'atualizaVersaoProjeto':
        ob_clean(); 
        $tipoReforma = $ctrPlanejamento->retornaCategoriaIniciativa();
        $obraTipo = new Par3_Model_ObraTipo($_REQUEST['otpid']);
        if($tipoReforma['octid'] == Par3_Model_ObraTipo::REFORMA && $obraTipo->otptipoprojeto == 'PP'){
            echo $simec->input('obralunos_atendidos', 'Alunos Atendidos', array(), array('required' => 'required', 'type' => 'number','max' => '50000'), array('input-size' => 2));
        }
        echo $simec->select('ovpid', 'Versão Projeto', $controllerObra->model->ovpid, $controllerObra->getComboVersaoProjeto($_POST), array('required' => 'required', 'class' => 'inteiro',$disableDadosBasicos));
        echo $controllerObra->campoMetragem($controllerObra->model->ovpid,$disableDadosBasicos);
        echo $simec->select('tplid', 'Versão Planilha', $controllerObra->model->tplid, array(), array('required' => 'required', 'class' => 'inteiro',$disableDadosBasicos));
        die();
        break;
    case 'atualizaVersaoPlanilha':
        ob_clean();
        echo $controllerObra->campoMetragem($_POST['ovpid'],$disableDadosBasicos);
        echo $simec->select('tplid', 'Versão Planilha', $controllerObra->model->tplid, $controllerObra->getComboVersaoPlanilha($_POST), array('required' => 'required', 'class' => 'inteiro',$disableDadosBasicos));
        die();
        break;
    default:
        $arrProj    = array();
        $arrVer     = array();
        if($controllerObra->model->otpid){
            $arrProj = $controllerObra->getComboVersaoProjeto(array('otpid'=>$controllerObra->model->otpid));
        }
        if($controllerObra->model->ovpid){
            $arrVer = $controllerObra->getComboVersaoPlanilha(array('ovpid'=>$controllerObra->model->ovpid));
        }
        $oct = $ctrPlanejamento->retornaCategoriaIniciativa();
        if($oct['octid'] != ''){
            $_REQUEST['octid'] = $oct['octid'];
            $tipoReadonoly = $oct['octid'] == Par3_Model_ObraTipo::REFORMA ? '' : 'disabled';
        }
        break;
}
    // Faço regra para exibição de Alunos Atendidos
    $obraTipo01 = new Par3_Model_ObraTipo($controllerObra->model->otpid);
    $categoriaIniciativa = $ctrPlanejamento->retornaCategoriaIniciativa();

;?>
<style>
    #map-canvas {
        height: 100%;
    }
    .readonly{
        border: 1px solid #B1AFAF !important;
        cursor: not-allowed;
        background-color: #eee;
    }
</style>

<link href='http://fonts.googleapis.com/css?family=Roboto:400,100,??100italic,300,300ita??lic,400italic,500,50??0italic,700,700itali??c,900italic,900'
      rel='stylesheet' type='text/css'>
<script src="<?= $urlApiMaps ?>"></script>

<div class="ibox" id="painelProgramas">

    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center"> Dados do Terreno </h2>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$controllerObra->model->obrid}") ?></h3>
<!--                <button id="extrato" class="btn btn-warning"><i class="fa fa-print">Extrato da Obra</i></button>-->
            </div>
        </div>
    </div>
    <form method="post" name="form-tipoObra" id="form-tipoObra" class="form form-horizontal">
        <div class="ibox-content">
            <div class="row">
                <input type="hidden" name="requisicao" value="salvar"/>
                <input type="hidden" name="obrid" id="obrid" value="<?php echo $_GET['obrid'] ?>"/>
                <input type="hidden" name="inuid" id="inuid" value="<?php echo $_GET['inuid'] ?>"/>
                <input type="hidden" name="tipoObra" id="tipoObra" value="<?php echo $obraTipo01->otptipoprojeto ?>"/>
                <input type="hidden" name="categoriaIniciativa" id="categoriaIniciativa" value="<?php echo $categoriaIniciativa['octid'] ?>"/>
                <?
                $mpergunta->carregarPergunta(Par3_Model_ObraAba::DADOS_TERRENO,1,$_GET['obrid']);
                ?>

                <div id="dados-basicos">
                    <?php
                    echo $simec->input('obrdsc', 'Nome', $controllerObra->model->obrdsc, array('required' => 'required', 'maxlength' => '255'));
                    echo $simec->select('otpid', 'Tipo de Obra', $controllerObra->model->otpid, $controllerObra->getComboTipoObra(), array('required' => 'required', $tipoReadonoly,  'class' => 'inteiro',$disableDadosBasicos));
                    echo $simec->input('obralunos_atendidos', 'Alunos Atendidos', $controllerObra->model->obralunos_atendidos, array('required' => 'required', 'type' => 'number','max' => '50000',$disableDadosBasicos), array('input-size' => 2));
                    echo $simec->select('ovpid', 'Versão Projeto', $controllerObra->model->ovpid, $arrProj, array('required' => 'required', 'class' => 'inteiro',$disableDadosBasicos));
                    echo $controllerObra->campoMetragem($controllerObra->model->ovpid,$disableDadosBasicos);
                    echo $simec->select('tplid', 'Versão Planilha', $controllerObra->model->tplid, $arrVer, array('required' => 'required', 'class' => 'inteiro',$disableDadosBasicos));
                    ?>
                </div>
                <?php if($pendenciaDadosBasicos):?>
                <script>
                    var dadosBasicos = $('#dados-basicos');
                        dadosBasicos.find('input,select').not('[id=otpid]').removeAttr('disabled');
                        dadosBasicos.find('.form-group').not('[id=otpid]').addClass('has-error');
                        dadosBasicos.find('input,select').not('[id=otpid]').change(function(){
                        var id = $(this).attr('id');
                        dadosBasicos.find('input,select').not('[id=otpid]').closest('.form-group').removeClass('has-error').addClass('has-success');
                    });
                </script>
                <?php endif;?>
                <hr>
                <h3 class="text-left">Endereço do Terreno</h3>
                <?php
                    $mpergunta->carregarPergunta(Par3_Model_ObraAba::DADOS_TERRENO,2,$_GET['obrid']);
                ?>
                <div id="endereco-terreno">
                    <?php


                    echo $simec->cep('obrcep', 'CEP', $controllerObra->model->obrcep, array('required' => 'required',$disableEndereco));
                    echo $simec->input('obrlogradouro', 'Logradouro', $controllerObra->model->obrlogradouro, array('required' => 'required','maxlength' => '255',$disableEndereco));
                    echo $simec->input('obrnumero', 'Número', $controllerObra->model->obrnumero, array('required' => 'required','type' => 'text','maxlength' => '15',$disableEndereco));
                    echo $simec->input('obrcomplemento', 'Complemento', $controllerObra->model->obrcomplemento, array('maxlength' => '255',$disableEndereco));
                    echo $simec->input('obrponto_referencia', 'Ponto de Referência', $controllerObra->model->obrponto_referencia, array('required' => 'required', 'maxlength' => '255',$disableEndereco));
                    echo $simec->input('obrbairro', 'Bairro', $controllerObra->model->obrbairro, array('required' => 'required','maxlength' => '100',$disableEndereco));
                    $mdMunicipio = new Territorios_Model_Municipio($mObra->muncod);
                    echo $simec->input('uf', 'Estado', $mdMunicipio->estuf, array('readonly' => 'readonly', 'maxlength' => '255',$disableEndereco));
                    echo $simec->input('municipio', 'Município', $mdMunicipio->mundescricao, array('readonly' => 'readonly', 'maxlength' => '255',$disableEndereco));
                    ?>
                </div>
                <?php if($pendenciaEndereco):?>
                    <script>
                        var  enderecoTerreno = $('#endereco-terreno');
                        enderecoTerreno.find('input,select').removeAttr('disabled');
                        enderecoTerreno.find('.form-group').addClass('has-error');
                        enderecoTerreno.find('input,select').change(function(){
                        var id = $(this).attr('id');
                        enderecoTerreno.find('input,select').closest('.form-group').removeClass('has-error').addClass('has-success');
                        });
                    </script>
                <?php endif;?>

                <input type="hidden" name="estuf" id="estuf" value="<?php echo $controllerObra->model->estuf;?>" />
                <input type="hidden" name="muncod" id="muncod" value="<?php echo $controllerObra->model->muncod;?>" />
                <hr>
                <?php
                    $mpergunta->carregarPergunta(Par3_Model_ObraAba::DADOS_TERRENO,3,$_GET['obrid']);
                ?>
                <div id="latitude-longitude">
                <?php
                echo $simec->input('obrlatitude', 'Latitude', $controllerObra->model->obrlatitude, array('required' => 'required', 'maxlength' => '12', 'autocomplete' => 'off', 'data-toggle' => 'tooltip', 'title' => 'Favor Selecionar o local no mapa abaixo!'));
                echo $simec->input('obrlongitude', 'Longitude', $controllerObra->model->obrlongitude, array('required' => 'required', 'maxlength' => '12', 'autocomplete' => 'off', 'data-toggle' => 'tooltip', 'title' => 'Favor Selecionar o local no mapa abaixo!'));
                ?>
                </div>
                <?php if($pendenciaLatitude):?>
                    <script>
                        var  latitudeLongitude = $('#latitude-longitude');
                        latitudeLongitude.find('.form-group').addClass('has-error');
                        latitudeLongitude.find('input,select').change(function(){
                        var id = $(this).attr('id');
                        latitudeLongitude.closest('.form-group').removeClass('has-error').addClass('has-success');
                        });
                    </script>
                <?php endif;?>
                <div class="form-group">
                    <span class="help-block m-b-none"><i class="fa fa-question-circle" style="color: #1c84c6;float:left; margin:-6px 317px;"></i> 
                        <h4 style="float:left; margin:-7px -309px;">Para informar os campos Latitude e Longitude do terreno, utilize o mapa abaixo.</h4>
                    </span>
                </div>
                <p class="text-center" style="margin: 40px;">
                    <a href="#" id="visualizarMapa"> <i class="fa fa-map-marker"></i> Visualizar / Buscar no mapa</a>
                </p>

                <?php echo $simec->input('campodeprocurar', 'Pesquisar localidade', '', array()); ?>
                <div id="map_canvas" style="height: 480px;width: 950px;margin: 20px; display: block; margin-left: auto; margin-right: auto"></div>
            </div>
        </div>
        <div class="ibox-footer">
            <div style="text-align: center;">
<!--                --><?php //if(!$disabled):?>
                <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
<!--                --><?php //endif;?>
            </div>
        </div>
    </form>
</div>
<script>
    var muncod_usuario = '<?php echo $controleUnidade->pegarMuncod($inuid); ?>';
    var estuf_usuario = '<?php echo $controleUnidade->pegarEstUF($inuid); ?>';
    $('#dados-basicos').find('input').parent('form-group').addClass('has-error');
    var tipoObra            = $("#tipoObra").val();
    var categoriaIniciativa = $("#categoriaIniciativa").val();
    if(tipoObra != 'PP' || categoriaIniciativa != <?php echo Par3_Model_ObraTipo::REFORMA ?>) { 
        $('#obralunos_atendidos').closest('.form-group ').hide();
        $('#obralunos_atendidos').prop('required',false);
    }

    $(document).ready(function () {
       $('input').keypress(function (e) {
            var code = null;
            code = (e.keyCode ? e.keyCode : e.which);
            return (code == 13) ? false : true;
       });

        $(".readonly").on('keydown',function(e){
            e.preventDefault();
        });

    });
    $('#otpid').change(function(){
        var otpid = $(this).val();

        if ($('#otpid').val().length <= 0) {
            alert("O campo Tipo de Obra é  obrigatório e não foi preenchido.");
            location.reload();
            return false;

        }

        var caminho = window.location.href;
        var action = '&requisicao=atualizaVersaoProjeto&otpid='+otpid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#obralunos_atendidos').parent().parent().remove();
                $('#ovpid').parent().parent().remove();
                $('#tplid').parent().parent().remove();
                $('#div_metragem').remove();
                $('#otpid').parent().parent().after(resp);
                return true;
            }
        });
    });

    $(document).on('change', '#ovpid', function(){
        var ovpid = $(this).val();
        if ($('#ovpid').val().length <= 0) {
            alert("O campo Versão Projeto é  obrigatório e não foi preenchido.");
            location.reload();
            return false;

        }
        var caminho = window.location.href;
        var action = '&requisicao=atualizaVersaoPlanilha&ovpid='+ovpid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#tplid').parent().parent().remove();
                $('#div_metragem').remove();
                $('#ovpid').parent().parent().after(resp);
                return true;
            }
        });
    });

    $('#obrcep').blur(function()
    {
        if ($('#obrcep').val() == '') {
            limparDadosEndereco();
            return false;
        }

        var boBuscaEndCep = true;

        var data = new Array();
        data.push({name : 'requisicao', value : 'verificaCepUF'},
            {name : 'db', value : true},
            {name : 'cep', value : $(this).val()}
        );

        $.ajax({
            type		: "POST",
            url		: window.location.href,
            data		: data,
            async    : false,
            success	: function(res){
                var estuf = res;
                if(trim(estuf) != '' && trim(estuf_usuario) != ''){
                    if(trim(estuf) != trim(estuf_usuario)){
                        alert('Favor informar um cep do UF cadastrado.');
                        limparDadosEndereco();
                        boBuscaEndCep = false;
                        return false;
                    }
                }
            }
        });

        if(boBuscaEndCep){
            var data = new Array();
            data.push({name : 'requisicao', value : 'verificaCepMunicipio'},
                {name : 'db', value : true},
                {name : 'cep', value : $(this).val()}
            );

            $.ajax({
                type		: "POST",
                url		: window.location.href,
                data		: data,
                async    : false,
                success	: function(res){
                    var muncod = res;
                    if(trim(muncod) != '' && trim(muncod_usuario) != ''){
                        if(trim(muncod) != trim(muncod_usuario)){
                            alert('Favor informar um cep do município cadastrado.');
                            limparDadosEndereco();
                            boBuscaEndCep = false;
                            return false;
                        }
                    }
                }
            });
            console.log(boBuscaEndCep);
            if(boBuscaEndCep){
                getEnderecoPeloCEP();
            }
        }
    });

    function limparDadosEndereco()
    {
        $('#obrlogradouro').val('');
        $('#obrnumero').val('');
        $('#obrcomplemento').val('');
        $('#obrponto_referencia').val('');
        $('#obrbairro').val('');
        $('#uf').val('');
        $('#muncod').val('');
    }

    function getEnderecoPeloCEP()
    {

        var data = new Array();
        data.push({name : 'opt', value : 'dne'},
            {name : 'endcep', value : $('#obrcep').val()}
        );
        console.log(data);
        $.ajax({
            type		: "POST",
            url		: '/geral/dne.php',
            data		: data,
            async    : false,
            success	: function(res){

                eval(res);

                if(DNE[0].cep != '' && DNE[0].logradouro ==  '' ){
                    DNE[0].logradouro = null;
                }

                if (!DNE[0].cep || DNE[0].cep == '') {
                    swal("Erro", 'CEP não encontrado!', "error")
                    $('#obrcep').val('');
                    return false;
                }

                $('#obrlogradouro').val(DNE[0].logradouro);
                $('#obrbairro').val(DNE[0].bairro);
                $('#uf').val(DNE[0].estado);
                $('#estuf').val(DNE[0].estado);
                $('#muncod').val(DNE[0].muncod);

                var data = new Array();
                data.push({name : 'requisicao', value : 'recuperaDescricaoMuncod'},
                    {name : 'muncod', value : DNE[0].muncod}
                );

                var boBuscaEndCep = true;
                $.ajax({
                    type		: "POST",
                    url		: window.location.href,
                    data		: data,
                    async    : false,
                    success	: function(res){
                        $('#municipio').val(res);
                    }
                });
            }
        });
    }

    $(function () {

        function initialize() {
            if($('#obrlatitude').val() && $('#obrlongitude').val() ){
                var lat = $('#obrlatitude').val(),
                    lng = $('#obrlongitude').val();
            }else{
                var lat = -15.799940193919426,
                    lng = -47.861337661743164;
            }
            var latlng = new google.maps.LatLng(lat, lng),
                image = 'http://www.google.com/intl/en_us/mapfiles/ms/micons/blue-dot.png';
            //zoomControl: true,
            //zoomControlOptions = google.maps.ZoomControlStyle.LARGE,

            var mapOptions = {
                    center: new google.maps.LatLng(lat, lng),
                    zoom: 10,
                    mapTypeId: google.maps.MapTypeId.ROADMAP,
                    panControl: true,
                    panControlOptions: {
                        position: google.maps.ControlPosition.TOP_RIGHT
                    },
                    zoomControl: true,
                    zoomControlOptions: {
                        style: google.maps.ZoomControlStyle.LARGE,
                        position: google.maps.ControlPosition.TOP_left
                    }
                },
                map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions),
                marker = new google.maps.Marker({
                    position: latlng,
                    map: map,
                    icon: image
                });

            var input = document.getElementById('campodeprocurar');
            var autocomplete = new google.maps.places.Autocomplete(input, {
                types: ["geocode"]
            });

            autocomplete.bindTo('bounds', map);

            var infowindow = new google.maps.InfoWindow();

            google.maps.event.addListener(autocomplete, 'place_changed', function (event) {
                infowindow.close();
                var place = autocomplete.getPlace();
                if (place.geometry.viewport) {
                    map.fitBounds(place.geometry.viewport);
                } else {
                    map.setCenter(place.geometry.location);
                    map.setZoom(10);
                }
                moveMarker(place.name, place.geometry.location);
                $('#obrlatitude').val(place.geometry.location.lat());
                $('#obrlongitude').val(place.geometry.location.lng());
            });

            google.maps.event.addListener(map, 'click', function (event) {
                $('#obrlatitude').val(event.latLng.lat());
                $('#obrlongitude').val(event.latLng.lng());
                infowindow.close();
                var geocoder = new google.maps.Geocoder();
                geocoder.geocode({
                    "latLng": event.latLng
                }, function (results, status) {
                    console.log(results, status);
                    if (status == google.maps.GeocoderStatus.OK) {
                        console.log(results);
                        var lat = results[0].geometry.location.lat(),
                            lng = results[0].geometry.location.lng(),
                            placeName = results[0].address_components[0].long_name,
                            latlng = new google.maps.LatLng(lat, lng);
                        moveMarker(placeName, latlng);
                        $("#campodeprocurar").val(results[0].formatted_address);
                    }
                });
            });

            function moveMarker(placeName, latlng) {
                marker.setIcon(image);
                marker.setPosition(latlng);
                infowindow.setContent(placeName);
//                 infowindow.open(map, marker);
            }
        }

        $("#map_canvas").toggle();
        $("#campodeprocurar").closest('.form-group').toggle();
        $('#visualizarMapa').on('click', function (e) {
            e.preventDefault();
            $("#campodeprocurar").closest('.form-group').toggle();
            $("#map_canvas").toggle();
            initialize()
        });
    });

    $("#extrato").on("click", function () {
        var action = '&requisicao=extrato';
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            dataType: 'html',
            async: false,
            success: function (resp) {
                var w = window.open();
                w.document.write(resp);
                w.document.title = 'Extrato de Obras';
                w.print();
                w.close();
                return true;
            }
        });
    });
</script>