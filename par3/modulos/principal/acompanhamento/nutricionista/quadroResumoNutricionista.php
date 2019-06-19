<style>
.fa-file-excel-o{
    font-size:15px;
    color: #2eb82e;
    cursor: pointer;
}
.fa-file-excel-o:hover{
    color: #47d147;
    -webkit-box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.5);
    -moz-box-shadow:    0px 0px 10px 0px rgba(0,0,0,0.5);
    box-shadow:         0px 0px 10px 0px rgba(0,0,0,0.5);
}
</style>
<div class="row">
    <div class="col-lg-1"></div>
    <div class="col-lg-10">
        <div class="ibox-title">
            <center>
                <h5>Quadro resumo de cadastro</h5>
            </center>
		</div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-6 text-right"><h5>Quantidade de municípios com cadastro</h5></div>
                <div class="col-lg-5 text-right"><?php echo $arrDados['qtd_mun_com_cadastro']?></div>
                <div class="col-lg-1">
                    <i class="fa fa-file-excel-o gera_xls" tipo="mcc"></i>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-6 text-right"><h5>Quantidade de municípios sem cadastro</h5></div>
                <div class="col-lg-5 text-right"><?php echo $arrDados['qtd_mun_sem_cadastro']?></div>
                <div class="col-lg-1">
                    <i class="fa fa-file-excel-o gera_xls" tipo="msc"></i>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-6 text-right"><h5>Quantidade de estados com cadastro</h5></div>
                <div class="col-lg-5 text-right"><?php echo $arrDados['qtd_est_com_cadastro']?></div>
                <div class="col-lg-1">
                    <i class="fa fa-file-excel-o gera_xls" tipo="ecc"></i>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-6 text-right"><h5>Quantidade de estados sem cadastro</h5></div>
                <div class="col-lg-5 text-right"><?php echo $arrDados['qtd_est_sem_cadastro']?></div>
                <div class="col-lg-1">
                    <i class="fa fa-file-excel-o gera_xls" tipo="esc"></i>
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-1"></div>
</div>